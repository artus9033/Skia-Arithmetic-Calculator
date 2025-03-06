#include "main.h"

/**
 * @brief Main application entry point
 * @return 0 on success, non-zero on failure
 */
int main() {
    spdlog::set_level(SET_LOG_ACTIVE_LEVEL);

    try {
        gui::window::GLFWWindowImpl<gui::renderer::SkiaRendererImpl, SkCanvas> window(
            "Flow Arithmetic Calculator");
        window.run();
    } catch (const std::exception& e) {
        return -1;
    }

    return 0;
}
