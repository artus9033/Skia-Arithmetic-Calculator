#ifndef BUSINESS_LOGIC_ELEMENTS_INTERACTIONS_IDOUBLECLICKABLE_H
#define BUSINESS_LOGIC_ELEMENTS_INTERACTIONS_IDOUBLECLICKABLE_H

namespace business_logic::elements::interactions {

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

}  // namespace business_logic::elements::interactions

#endif  // BUSINESS_LOGIC_ELEMENTS_INTERACTIONS_IDOUBLECLICKABLE_H
