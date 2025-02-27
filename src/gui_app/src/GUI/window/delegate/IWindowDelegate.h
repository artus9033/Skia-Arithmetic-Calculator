#ifndef GUI_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H
#define GUI_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H

#include "GUI/geometry/Size2D.h"

namespace gui::window::delegate {
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
         */
        virtual void focusWindow() = 0;
    };
}  // namespace gui::window::delegate

#endif  // GUI_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H
