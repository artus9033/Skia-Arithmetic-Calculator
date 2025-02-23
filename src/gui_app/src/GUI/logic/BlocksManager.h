#ifndef GUI_LOGIC_BLOCKS_MANAGER_H
#define GUI_LOGIC_BLOCKS_MANAGER_H

#include <algorithm>
#include <boost/range/adaptor/reversed.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "GUI/elements/base/BaseBlock.h"
#include "constants.h"

namespace gui::logic {
    /**
     * @brief Manages the blocks in the GUI, both their rendering and interaction
     */
    class BlocksManager {
       public:
        BlocksManager();

        /**
         * @brief Handles the mouse down event
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        void handleMouseDown();

        /**
         * @brief Handles the mouse up event
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        void handleMouseUp();

        /**
         * @brief Handles the mouse move event
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        void handleMouseMove(int x, int y);

        /**
         * @brief Renders the blocks on the canvas
         * @param canvas SkCanvas to draw on
         */
        void render(SkCanvas* canvas);

        /**
         * @brief Gets the block at the mouse position
         * @return The block at the mouse position, or `std::nullopt` if no block is at the mouse
         * position
         */
        std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> getBlockAtMousePos();

        /**
         * @brief Gets the block at the given coordinates
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         * @return The block at the given coordinates, or `std::nullopt` if no block is at the given
         * coordinates
         */
        std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> getBlockAt(int x, int y);

       protected:
        /**
         * The blocks in the manager
         */
        std::vector<std::shared_ptr<gui::elements::base::BaseBlock>> blocks;

        /**
         * The block that is currently being dragged
         */
        std::shared_ptr<gui::elements::base::BaseBlock> draggedBlock;

        /**
         * The block that is currently being hovered over
         */
        std::shared_ptr<gui::elements::base::BaseBlock> hoveredBlock;

        /**
         * The mouse X coordinate
         */
        int mouseX;

        /**
         * The mouse Y coordinate
         */
        int mouseY;

        /**
         * The time of the last mouse click
         */
        time_t lastMouseClickTime;

        /**
         * @brief Renders the dragged line on the canvas
         * @param canvas SkCanvas to draw on
         */
        void maybeRenderDraggedLine(SkCanvas* canvas) const;
    };

}  // namespace gui::logic

#endif  // GUI_LOGIC_BLOCKS_MANAGER_H
