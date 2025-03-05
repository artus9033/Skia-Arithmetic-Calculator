#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>

#include "GUI/renderer/IRenderer.h"
#include "logging/Loggable.h"
#include "utils/misc.h"

namespace gui::window {
    /**
     * @tparam RendererImpl The renderer implementation type
     * @brief SFINAE guard to make sure T conforms to IRenderer
     */
    template <typename RendererImpl>
    concept IsIRenderer = std::is_base_of_v<renderer::IRenderer, RendererImpl>;

    /**
     * @brief Class responsible for managing the GLFW window and its lifecycle
     * @tparam RendererImpl The renderer implementation type, must conform to `IsIRenderer`
     * @see IsIRenderer
     * @see IRenderer
     */
    template <IsIRenderer RendererImpl>
    class GLFWWindowImpl : protected business_logic::Loggable<GLFWWindowImpl<RendererImpl>>,
                           public window::WindowBase {
       private:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<GLFWWindowImpl<RendererImpl>>::logger;

        GLFWwindow* glfwWindow;
        std::unique_ptr<renderer::IRenderer> renderer;
        static bool initializedGLFW;

       public:
        /**
         * @brief Constructs a new GLFWWindowImpl
         * @param width Initial window width
         * @param height Initial window height
         * @param title GLFWWindowImpl title
         */
        explicit GLFWWindowImpl(int width, int height, const char* title)
            : business_logic::Loggable<GLFWWindowImpl<RendererImpl>>(), window::WindowBase(logger) {
            initGLFW();

            initializeGLFWWindow(width, height, title);

            postInitialize();
        }

        /**
         * @brief Constructs a new GLFWWindowImpl that takes the full size of the primary monitor
         * @param title GLFWWindowImpl title
         */
        explicit GLFWWindowImpl(const char* title)
            : business_logic::Loggable<GLFWWindowImpl<RendererImpl>>(), window::WindowBase(logger) {
            initGLFW();

            // Get primary monitor
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            if (!primaryMonitor) {
                throw std::runtime_error("Failed to get primary monitor");
            }

            // Get video mode (which contains screen width and height)
            const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
            if (!videoMode) {
                throw std::runtime_error("Failed to get primary monitor's video mode");
            }

            initializeGLFWWindow(
                std::round(videoMode->width * 0.6), std::round(videoMode->height * 0.6), title);

            postInitialize();
        };

        GLFWWindowImpl(const GLFWWindowImpl&) = delete;
        GLFWWindowImpl& operator=(const GLFWWindowImpl&) = delete;

        void postInitialize() {
            // initially invoke window->handleMouseMove
            double mouseX, mouseY;
            glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);

            handleMouseMove(mouseX, mouseY);
        }

        /**
         * @brief Destructor that cleans up the GLFW window and terminates GLFW
         */
        ~GLFWWindowImpl() {
            logger->info("GLFWWindowImpl {} has been destroyed",
                         business_logic::stringifyAddressOf(this));

            if (glfwWindow) {
                glfwDestroyWindow(glfwWindow);
            }
            glfwTerminate();
        }

        /**
         * \copydoc WindowBase::run
         */
        void run() override {
            while (!shouldClose()) {
                glfwPollEvents();

                renderer->render();

                // swap front and back buffers
                glfwSwapBuffers(glfwWindow);
            }

            logger->info("shouldClose() returned true, exiting gracefully");
        };

        /**
         * @brief Allows conversion of GLFWWindowImpl instance that gets the underlying GLFW window
         * handle
         * @return GLFWwindow* The window handle
         */
        operator GLFWwindow*() const { return glfwWindow; }

       private:
        /**
         * @brief Initializes GLFW
         */
        void initGLFW() {
            if (!initializedGLFW) {
                logger->info("Initializing GLFW");

                if (glfwInit()) {
                    glfwSetErrorCallback([](int error, const char* description) {
                        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
                    });

                    logger->info("Initialized GLFW");

                    initializedGLFW = true;
                } else {
                    throw std::runtime_error("Failed to initialize GLFW");
                }
            }
        };

        /**
         * \copydoc WindowBase::shouldClose
         */
        bool shouldClose() const override { return glfwWindowShouldClose(glfwWindow); }

        /**
         * @brief Initializes the GLFW window
         * @param width The width of the window
         * @param height The height of the window
         * @param title The title of the window
         */
        void initializeGLFWWindow(int width, int height, const char* title) {
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_STENCIL_BITS, 0);
            glfwWindowHint(GLFW_DEPTH_BITS, 0);

            glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

            logger->info(
                "Creating window with title: {} and initial size: {}x{}", title, width, height);

            if (!glfwWindow) {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW window");
            }

            glfwMakeContextCurrent(glfwWindow);
            glfwSwapInterval(1);
            glfwSetWindowUserPointer(glfwWindow, this);
            glfwSetWindowSizeCallback(glfwWindow,
                                      [](GLFWwindow* glfwWindow, int winWidth, int winHeight) {
                                          handleWindowResized(glfwWindow, winWidth, winHeight);
                                      });

            glfwSetKeyCallback(
                glfwWindow,
                [](GLFWwindow* glfwWindow,
                   int key,
                   [[maybe_unused]] int scancode,
                   int action,
                   [[maybe_unused]] int mods) {
                    if (action == GLFW_PRESS) {
                        if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
                            WindowBase* window =
                                static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));

                            int number = key - GLFW_KEY_0;
                            window->handleNumericKeyPress(number);
                        } else if (key == GLFW_KEY_ESCAPE) {
                            WindowBase* window =
                                static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));

                            window->handleEscapeKeyPress();
                        }
                    }
                });

            glfwSetMouseButtonCallback(
                glfwWindow,
                [](GLFWwindow* glfwWindow, int button, int action, [[maybe_unused]] int mods) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                        WindowBase* window =
                            static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));

                        window->handleMouseDown();
                    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
                        WindowBase* window =
                            static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));

                        window->handleMouseUp();
                    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                        WindowBase* window =
                            static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));

                        window->handleRightClick();
                    }
                });
            glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double x, double y) {
                WindowBase* window = static_cast<WindowBase*>(glfwGetWindowUserPointer(glfwWindow));

                window->handleMouseMove(x, y);
            });

            int fbWidth, fbHeight;
            glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeight);

            renderer = std::make_unique<RendererImpl>(
                this, width, height, fbWidth, fbHeight, blocksManager);

            handleWindowResized(glfwWindow, width, height);
        }

        /**
         * @brief Handles the window resized event
         * @param window The GLFW window
         * @param winWidth The width of the window
         * @param winHeight The height of the window
         */
        static void handleWindowResized(GLFWwindow* window, int winWidth, int winHeight) {
            auto* self = static_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(window));

            if (self && self->renderer) {
                int fbWidth, fbHeight;
                glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

                self->winSize = geometry::Size2D(winWidth, winHeight);
                self->framebufferSize = geometry::Size2D(fbWidth, fbHeight);

                double xScale = static_cast<double>(fbWidth) / winWidth;
                double yScale = static_cast<double>(fbHeight) / winHeight;

                self->logger->info(
                    "Informing renderer {} of window size change to {}x{}, framebuffer size to "
                    "{}x{} "
                    "and scales to {:.2f}x{:.2f}",
                    business_logic::stringifyAddressOf(self->renderer.get()),
                    winWidth,
                    winHeight,
                    fbWidth,
                    fbHeight,
                    xScale,
                    yScale);

                self->renderer->handleWindowResized(self, xScale, yScale);
            }
        }

        /**
         * \copydoc WindowBase::focusWindow
         */
        void focusWindow() override { glfwFocusWindow(glfwWindow); }
    };
}  // namespace gui::window

#endif  // WINDOW_H
