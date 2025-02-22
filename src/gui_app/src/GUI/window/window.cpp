#include "window.h"

Window::Window(int width, int height, const char* title) {
    initGLFW();

    initializeGLFWWindow(width, height, title);
}

Window::Window(const char* title) {
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
}

void Window::initializeGLFWWindow(int width, int height, const char* title) {
    glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!glfwWindow) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(glfwWindow);
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, framebufferSizeCallback);

    m_renderer = std::make_unique<SkiaRenderer>(this, width, height);
}

void Window::initGLFW() {
    if (!initializedGLFW) {
        if (glfwInit()) {
            initializedGLFW = true;
        } else {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }
}

Window::~Window() {
    if (glfwWindow) {
        glfwDestroyWindow(glfwWindow);
    }
    glfwTerminate();
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    auto* self = getWrapperForGLFWWindow(window);

    if (self && self->m_renderer) {
        self->m_renderer->handleWindowResized(width, height);
    }
}

Window* Window::getWrapperForGLFWWindow(GLFWwindow* window) {
    return static_cast<Window*>(glfwGetWindowUserPointer(window));
}

bool Window::shouldClose() const { return glfwWindowShouldClose(glfwWindow); }

void Window::run() {
    while (!shouldClose()) {
        glfwPollEvents();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        m_renderer->render();

        // swap front and back buffers
        glfwSwapBuffers(glfwWindow);
    }
}

bool Window::initializedGLFW = false;
