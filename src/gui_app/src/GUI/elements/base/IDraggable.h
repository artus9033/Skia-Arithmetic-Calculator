#pragma once

namespace gui::elements::base {

    /**
     * @brief Interface for elements that can be dragged
     */
    class IDraggable {
       public:
        virtual ~IDraggable() = default;

        /**
         * @brief Called when dragging starts
         */
        virtual void onDragStart() = 0;

        /**
         * @brief Called during dragging
         * @param x Current X coordinate
         * @param y Current Y coordinate
         */
        virtual void onDragProgress(int x, int y) = 0;

        /**
         * @brief Called when dragging ends
         */
        virtual void onDragEnd() = 0;
    };

}  // namespace gui::elements::base
