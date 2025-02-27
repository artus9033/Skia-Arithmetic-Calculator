#include "BaseBlock.h"

namespace gui::elements::base {

    BaseBlock::BaseBlock(int cx,
                         int cy,
                         int blockWidth,
                         int blockHeight,
                         std::shared_ptr<spdlog::logger> logger,
                         const geometry::Size2D& windowSize)
        : width(blockWidth), height(blockHeight), logger(logger) {
        int leftTopX = std::min(std::max(0, cx - blockWidth / 2), windowSize.width - blockWidth);
        int leftTopY = std::min(std::max(0, cy - blockHeight / 2), windowSize.height - blockHeight);

        cacheBottomRightCorner();

        x = leftTopX;
        y = leftTopY;
    }

    void BaseBlock::updateWidthHeight(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;

        cacheBottomRightCorner();
    }

    void BaseBlock::onDragProgress(int x, int y) {
        logger->info("Block {} has been dragged", getSelfId());

        this->x = x;
        this->y = y;
    }

    void BaseBlock::onDragStart(int x, int y) {
        logger->info("Block {} has been dragged", getSelfId());

        this->x = x;
        this->y = y;
    }

    void BaseBlock::onDragEnd(int x, int y) {
        logger->info("Block {} has been destroyed", getSelfId());

        this->x = x;
        this->y = y;
    }

    void BaseBlock::cacheBottomRightCorner() {
        bottomRightX = x - width / 2;
        bottomRightY = y - height / 2;
    }

}  // namespace gui::elements::base
