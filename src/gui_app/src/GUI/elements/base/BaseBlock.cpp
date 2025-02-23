#include "BaseBlock.h"

namespace gui::elements::base {

    BaseBlock::BaseBlock(
        int cx, int cy, int blockWidth, int blockHeight, std::shared_ptr<spdlog::logger> logger)
        : width(blockWidth), height(blockHeight), logger(logger) {
        int leftTopX = cx - blockWidth / 2;
        int leftTopY = cy - blockHeight / 2;

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
