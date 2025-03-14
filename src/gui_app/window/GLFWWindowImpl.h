#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <stdexcept>

#include "WindowBase.h"
#include "logging/Loggable.h"
#include "utils/misc.h"
#include "window/prompt/GuiMessageBox.h"
#include "window/prompt/TextInputDialog.h"

namespace gui::window {
    /**
     * @brief Class responsible for managing the GLFW window and its lifecycle
     *
     * @tparam RendererImpl The renderer implementation type
     * @tparam Canvas The canvas type
     */
    template <typename RendererImpl, typename Canvas>
    class GLFWWindowImpl : protected business_logic::Loggable<GLFWWindowImpl<RendererImpl, Canvas>>,
                           public window::WindowBase<Canvas> {
       public:
        /**
         * @brief Constructs a new GLFWWindowImpl
         * @param width Initial window width
         * @param height Initial window height
         * @param title GLFWWindowImpl title
         */
        explicit GLFWWindowImpl(int width, int height, const char* title)
            : business_logic::Loggable<GLFWWindowImpl<RendererImpl, Canvas>>(),
              window::WindowBase<Canvas>(width, height, logger) {
            initGLFW();

            initializeGLFWWindow(width, height, title);

            postInitialize();
        }

        /**
         * @brief Constructs a new GLFWWindowImpl that takes the full size of the primary monitor
         * @param title GLFWWindowImpl title
         */
        static GLFWWindowImpl<RendererImpl, Canvas> MakeFullscreen(const char* title) {
            initGLFW();

            // Get primary monitor
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            if (primaryMonitor == nullptr) {
                throw std::runtime_error("Failed to get primary monitor");
            }

            // Get video mode (which contains screen width and height)
            const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
            if (videoMode == nullptr) {
                throw std::runtime_error("Failed to get primary monitor's video mode");
            }

            return GLFWWindowImpl<RendererImpl, Canvas>(
                std::round(videoMode->width * 0.6), std::round(videoMode->height * 0.6), title);
        };

        // disable copy semantics
        GLFWWindowImpl(const GLFWWindowImpl&) = delete;
        GLFWWindowImpl& operator=(const GLFWWindowImpl&) = delete;

        // disable move semantics
        GLFWWindowImpl(GLFWWindowImpl&&) = delete;
        GLFWWindowImpl& operator=(GLFWWindowImpl&&) = delete;

        void postInitialize() {
            // initially invoke window->handleMouseMove
            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
            double mouseX;
            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
            double mouseY;
            glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);

            this->handleMouseMove(mouseX, mouseY);
        }

        /**
         * @brief Destructor that cleans up the GLFW window and terminates GLFW
         */
        ~GLFWWindowImpl() override {
            logger->info("GLFWWindowImpl {} has been destroyed",
                         business_logic::stringifyAddressOf(this));

            if (glfwWindow != nullptr) {
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
        explicit operator GLFWwindow*() const { return glfwWindow; }

       private:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<GLFWWindowImpl<RendererImpl, Canvas>>::logger;

        GLFWwindow* glfwWindow{};
        inline static bool initializedGLFW;

        std::unique_ptr<RendererImpl> renderer;

        /**
         * @brief Initializes GLFW
         */
        static void initGLFW() {
            if (!initializedGLFW) {
                std::cout << "Initializing GLFW" << std::endl;

                if (glfwInit() != 0) {
                    glfwSetErrorCallback([](int error, const char* description) {
                        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
                    });

                    std::cout << "Initialized GLFW" << std::endl;

                    initializedGLFW = true;
                } else {
                    throw std::runtime_error("Failed to initialize GLFW");
                }
            }
        };

        /**
         * \copydoc WindowBase::shouldClose
         */
        bool shouldClose() const override { return glfwWindowShouldClose(glfwWindow) != 0; }

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

            if (glfwWindow == nullptr) {
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

            glfwSetKeyCallback(glfwWindow,
                               [](GLFWwindow* glfwWindow,
                                  int key,
                                  [[maybe_unused]] int scancode,
                                  int action,
                                  [[maybe_unused]] int mods) {
                                   if (action == GLFW_PRESS) {
                                       if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
                                           auto* window = static_cast<WindowBase<Canvas>*>(
                                               glfwGetWindowUserPointer(glfwWindow));

                                           int number = key - GLFW_KEY_0;
                                           window->handleNumericKeyPress(number);
                                       } else if (key == GLFW_KEY_ESCAPE) {
                                           auto* window = static_cast<WindowBase<Canvas>*>(
                                               glfwGetWindowUserPointer(glfwWindow));

                                           window->handleEscapeKeyPress();
                                       }
                                   }
                               });

            glfwSetMouseButtonCallback(
                glfwWindow,
                [](GLFWwindow* glfwWindow, int button, int action, [[maybe_unused]] int mods) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
                        auto* window =
                            static_cast<WindowBase<Canvas>*>(glfwGetWindowUserPointer(glfwWindow));

                        window->handleMouseDown();
                    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
                        auto* window =
                            static_cast<WindowBase<Canvas>*>(glfwGetWindowUserPointer(glfwWindow));

                        window->handleMouseUp();
                    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
                        auto* window =
                            static_cast<WindowBase<Canvas>*>(glfwGetWindowUserPointer(glfwWindow));

                        window->handleRightClick();
                    }
                });
            glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double x, double y) {
                auto* window =
                    static_cast<WindowBase<Canvas>*>(glfwGetWindowUserPointer(glfwWindow));

                window->handleMouseMove(x, y);
            });

            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
            int fbWidth;
            // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
            int fbHeight;
            glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeight);

            renderer = std::make_unique<RendererImpl>(this, this->blocksManager);

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
                // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                int fbWidth;
                // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
                int fbHeight;
                glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

                self->winSize = {.width = winWidth, .height = winHeight};
                self->framebufferSize = {.width = fbWidth, .height = fbHeight};

                float xScale = static_cast<float>(fbWidth) / static_cast<float>(winWidth);
                float yScale = static_cast<float>(fbHeight) / static_cast<float>(winHeight);

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

        /**
         * \copydoc business_logic::delegate::IWindowDelegate::showWarning
         */
        void showWarning(const std::string& title, const std::string& message) override {
            gui::window::prompt::GuiMessageBox::showWarning(title, message, this);
        }

        /**
         * \copydoc business_logic::delegate::IWindowDelegate::promptFloatingPointInput
         */
        std::optional<FloatingPoint> promptFloatingPointInput(
            const std::string& title,
            const std::string& message,
            const std::optional<FloatingPoint>& defaultValue) override {
            return gui::window::prompt::TextInputDialog::promptForFloatingPointInput(
                title, message, defaultValue, this);
        }

        /**
         * \copydoc business_logic::delegate::IWindowDelegate::promptConfirmation
         */
        bool promptConfirmation(const std::string& title, const std::string& message) override {
            return gui::window::prompt::GuiMessageBox::promptConfirmation(title, message, this);
        }
    };
}  // namespace gui::window

#endif  // WINDOW_H
