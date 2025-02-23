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
         * @param x Initial X coordinate
         * @param y Initial Y coordinate
         */
        virtual void onDragStart(int x, int y) = 0;

        /**
         * @brief Called during dragging
         * @param x Current X coordinate
         * @param y Current Y coordinate
         */
        virtual void onDragProgress(int x, int y) = 0;

        /**
         * @brief Called when dragging ends
         * @param x Final X coordinate
         * @param y Final Y coordinate
         */
        virtual void onDragEnd(int x, int y) = 0;
    };

}  // namespace gui::elements::base
