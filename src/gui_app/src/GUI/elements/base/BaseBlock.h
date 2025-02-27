#ifndef GUI_ELEMENTS_BASE_I_BLOCK_H
#define GUI_ELEMENTS_BASE_I_BLOCK_H

#include <skia/include/core/SkCanvas.h>

#include "GUI/geometry/Size2D.h"
#include "IDraggable.h"
#include "logging/Loggable.h"

namespace gui::elements::base {
    class BaseBlock : public IDraggable {
       public:
        /**
         * @brief Constructor
         * @param cx The center x coordinate where to place the center of the block
         * @param cy The center y coordinate where to place the center of the block
         * @param blockWidth The width of the block
         * @param blockHeight The height of the block
         * @param windowSize The size of the window
         */
        BaseBlock(int cx,
                  int cy,
                  int blockWidth,
                  int blockHeight,
                  std::shared_ptr<spdlog::logger> logger,
                  const geometry::Size2D& windowSize);

        /**
         * @brief Updates the width and height of the block
         * @param newWidth The new width of the block
         * @param newHeight The new height of the block
         */
        void updateWidthHeight(int newWidth, int newHeight);

        virtual ~BaseBlock() = default;

        /**
         * @brief Checks if the block is hovered over by the mouse
         * @param x The x coordinate of the mouse
         * @param y The y coordinate of the mouse
         * @return True if the block is hovered over, false otherwise
         */
        virtual bool isHovered(int x, int y) const {
            return this->x >= x && x <= bottomRightX && this->y >= y && y <= bottomRightY;
        }

        /**
         * @brief Renders the block
         * @param canvas The canvas to render the block on
         * @param isHovered True if the block is hovered over, false otherwise
         */
        virtual void render(SkCanvas* canvas, bool isHovered) const = 0;

        /**
         * \copydoc IDraggable::onDragStart
         */
        void onDragStart(int x, int y) override;

        /**
         * \copydoc IDraggable::onDragProgress
         */
        void onDragProgress(int x, int y) override;

        /**
         * \copydoc IDraggable::onDragEnd
         */
        void onDragEnd(int x, int y) override;

       protected:
        /**
         * The x coordinate of the block
         */
        int x;

        /**
         * The y coordinate of the block
         */
        int y;

        /**
         * The width of the block
         */
        int width;

        /**
         * The height of the block
         */
        int height;

        /**
         * The bottom right corner of the block; cached and used for the needs of `isHovered()`
         */
        int bottomRightX;

        /**
         * The bottom right corner of the block; cached and used for the needs of `isHovered()`
         */
        int bottomRightY;

        /**
         * The unique identifier of the block (its address in memory), used for logging
         * purposes; should return the same value as `business_logic::stringifyAddressOf(this)`
         */
        virtual std::string getSelfId() const = 0;

        /**
         * The logger for the block
         */
        std::shared_ptr<spdlog::logger> logger;

        /**
         * @brief Caches the bottom right corner of the block, so that it is not recalculated every
         * time `isHovered()` is called
         */
        void cacheBottomRightCorner();
    };
}  // namespace gui::elements::base

#endif  // GUI_ELEMENTS_BASE_I_BLOCK_H
