#include <stdexcept>

#include "GUI/window/window.h"

/**
 * @brief Main application entry point
 * @return 0 on success, non-zero on failure
 */
int main() {
    try {
        Window window("Flow Arithmetic Calculator");
        window.run();
    } catch (const std::exception& e) {
        return -1;
    }

    return 0;
}
