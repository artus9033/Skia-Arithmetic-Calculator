#pragma once

namespace gui::elements::base {

    /**
     * @brief Interface for elements that can be double-clicked
     */
    class IDoubleClickable {
       public:
        virtual ~IDoubleClickable() = default;

        /**
         * @brief Called when the element is double-clicked
         * @param x X coordinate of the 2nd double-click event
         * @param y Y coordinate of the 2nd double-click event
         */
        virtual void onDoubleClick(int x, int y) = 0;
    };

}  // namespace gui::elements::base
