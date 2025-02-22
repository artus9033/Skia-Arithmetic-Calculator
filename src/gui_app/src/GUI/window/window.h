#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>

#include "GUI/renderer/skia_renderer.h"

class SkiaRenderer;

/**
 * @brief Class responsible for managing the GLFW window and its lifecycle
 */
class Window {
   public:
    /**
     * @brief Constructs a new Window
     * @param width Initial window width
     * @param height Initial window height
     * @param title Window title
     */
    Window(int width, int height, const char* title);

    /**
     * @brief Constructs a new Window that takes the full size of the primary monitor
     * @param title Window title
     */
    Window(const char* title);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    /**
     * @brief Destructor
     */
    ~Window();

    /**
     * @brief Runs the main window loop
     */
    void run();

    /**
     * @brief Allows conversion of Window instance that gets the underlying GLFW window handle
     * @return GLFWwindow* The window handle
     */
    operator GLFWwindow*() const { return glfwWindow; }

   private:
    /**
     * @brief Static callback for window resize events
     */
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    /**
     * @brief Initializes GLFW
     */
    void initGLFW();

    /**
     * @brief Gets the Window abstraction wrapper instance pointer for the given GLFW window
     * @param window The GLFW window
     * @return Window* The wrapper
     */
    static Window* getWrapperForGLFWWindow(GLFWwindow* window);

    /**
     * @brief Whether the window should close
     * @return bool True if the window should close, false otherwise
     */
    bool shouldClose() const;

    /**
     * @brief Initializes the GLFW window
     * @param width The width of the window
     * @param height The height of the window
     * @param title The title of the window
     */
    void initializeGLFWWindow(int width, int height, const char* title);

    GLFWwindow* glfwWindow;
    std::unique_ptr<SkiaRenderer> m_renderer;
    static bool initializedGLFW;
};

#endif  // WINDOW_H
