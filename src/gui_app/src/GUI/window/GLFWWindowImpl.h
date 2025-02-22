#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>

#include "GUI/renderer/IRenderer.h"
#include "logging/Loggable.h"
#include "utils/misc.h"

namespace gui::window {

    // SFINAE guard to make sure T conforms to IRenderer
    template <typename RendererImpl>
    concept IsIRenderer = std::is_base_of_v<renderer::IRenderer, RendererImpl>;

    /**
     * @brief Class responsible for managing the GLFW window and its lifecycle
     */
    template <IsIRenderer RendererImpl>
    class GLFWWindowImpl : protected business_logic::Loggable, public window::IWindow {
       private:
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
            : business_logic::Loggable("GLFWWindowImpl"), window::IWindow() {
            initGLFW();

            initializeGLFWWindow(width, height, title);
        }

        /**
         * @brief Constructs a new GLFWWindowImpl that takes the full size of the primary monitor
         * @param title GLFWWindowImpl title
         */
        explicit GLFWWindowImpl(const char* title)
            : business_logic::Loggable("GLFWWindowImpl"), window::IWindow() {
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

            initializeGLFWWindow(videoMode->width, videoMode->height, title);
        };

        GLFWWindowImpl(const GLFWWindowImpl&) = delete;
        GLFWWindowImpl& operator=(const GLFWWindowImpl&) = delete;

        /**
         * @brief Destructor
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
         * @brief Runs the main window loop
         */
        void run() override {
            while (!shouldClose()) {
                glfwPollEvents();

                // Clear the screen
                glClear(GL_COLOR_BUFFER_BIT);

                renderer->render();

                // swap front and back buffers
                glfwSwapBuffers(glfwWindow);
            }
        };

        /**
         * @brief Allows conversion of GLFWWindowImpl instance that gets the underlying GLFW window
         * handle
         * @return GLFWwindow* The window handle
         */
        operator GLFWwindow*() const { return glfwWindow; }

       private:
        /**
         * @brief Static callback for window resize events
         */
        static void staticFramebufferSizeHandler(GLFWwindow* window, int width, int height);

        /**
         * @brief Initializes GLFW
         */
        void initGLFW() {
            if (!initializedGLFW) {
                logger->info("Initializing GLFW");

                if (glfwInit()) {
                    logger->info("Initialized GLFW");

                    initializedGLFW = true;
                } else {
                    throw std::runtime_error("Failed to initialize GLFW");
                }
            }
        };

        /**
         * @brief Whether the window should close
         * @return bool True if the window should close, false otherwise
         */
        bool shouldClose() const override { return glfwWindowShouldClose(glfwWindow); }

        /**
         * @brief Initializes the GLFW window
         * @param width The width of the window
         * @param height The height of the window
         * @param title The title of the window
         */
        void initializeGLFWWindow(int width, int height, const char* title) {
            glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

            logger->info(
                "Creating window with title: {} and initial size: {}x{}", title, width, height);

            if (!glfwWindow) {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW window");
            }

            glfwMakeContextCurrent(glfwWindow);
            glfwSetWindowUserPointer(glfwWindow, this);
            glfwSetFramebufferSizeCallback(
                glfwWindow, [](GLFWwindow* window, int width, int height) {
                    auto* self = static_cast<GLFWWindowImpl*>(glfwGetWindowUserPointer(window));

                    if (self && self->renderer) {
                        self->logger->info("Informing renderer {} of window size change to {}x{}",
                                           business_logic::stringifyAddressOf(self->renderer.get()),
                                           width,
                                           height);

                        self->renderer->handleWindowResized(width, height);
                    }
                });

            renderer = std::make_unique<RendererImpl>(this, width, height);
        }
    };
}  // namespace gui::window

#endif  // WINDOW_H
