#pragma once

#include <functional>

namespace gui::elements::base {

    /**
     * @brief Interface for elements that can be clicked
     */
    class IClickable {
       public:
        virtual ~IClickable() = default;

        /**
         * @brief Called when the mouse is released on the element
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        virtual void onMouseUp(int x, int y) = 0;

        /**
         * @brief Called when the mouse is pressed down on the element
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        virtual void onMouseDown(int x, int y) = 0;
    };

}  // namespace gui::elements::base
