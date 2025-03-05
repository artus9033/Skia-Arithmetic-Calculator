#ifndef GUI_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H
#define GUI_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H

#include "GUI/geometry/Size2D.h"

namespace gui::window::delegate {
    /**
     * @brief The delegate for the window
     *
     * The delegate is used for basic window operations.
     */
    class IWindowDelegate {
       public:
        virtual ~IWindowDelegate() = default;

        /**
         * @brief Get the window size
         * @return The window size
         */
        virtual geometry::Size2D getWindowSize() = 0;

        /**
         * @brief Get the framebuffer size
         * @return The framebuffer size
         */
        virtual geometry::Size2D getFramebufferSize() = 0;

        /**
         * @brief Focus the window
         *
         * Helpful e.g. when closing a Qt dialog.
         */
        virtual void focusWindow() = 0;
    };
}  // namespace gui::window::delegate

#endif  // GUI_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H
