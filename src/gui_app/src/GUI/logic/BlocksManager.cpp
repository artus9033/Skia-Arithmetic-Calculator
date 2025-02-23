#include "BlocksManager.h"

namespace gui::logic {

    BlocksManager::BlocksManager() { lastMouseClickTime = 0; }

    void BlocksManager::handleMouseDown() { lastMouseClickTime = time(nullptr); }

    void BlocksManager::handleMouseUp() { lastMouseClickTime = 0; }

    void BlocksManager::handleMouseMove(int x, int y) {
        mouseX = x;
        mouseY = y;
    }

    std::optional<std::shared_ptr<gui::elements::base::BaseBlock>>
    BlocksManager::getBlockAtMousePos() {
        return getBlockAt(mouseX, mouseY);
    }

    std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> BlocksManager::getBlockAt(
        int x, int y) {
        // find the block that is being hovered over; reverse order to render newer blocks on top
        // (as if they had the highest z-index)
        auto blocksReversed = boost::adaptors::reverse(blocks);

        auto maybeHoveredBlockIt =
            std::find_if(blocksReversed.begin(), blocksReversed.end(), [x, y](const auto& block) {
                return block->isHovered(x, y);
            });

        std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> maybeHoveredBlock =
            std::nullopt;

        if (maybeHoveredBlockIt != blocksReversed.end()) {
            maybeHoveredBlock = *maybeHoveredBlockIt;
        }

        return maybeHoveredBlock;
    }

    void BlocksManager::render(SkCanvas* canvas) {
        auto maybeHoveredBlock = getBlockAtMousePos();

        if (maybeHoveredBlock.has_value()) {
            hoveredBlock = maybeHoveredBlock.value();  // update the state
        }

        // iterate from oldest to newest (normal order) to render newer ones on top of older ones
        for (const auto& block : blocks) {
            auto isFocused = maybeHoveredBlock.has_value() && (maybeHoveredBlock == block);

            block->render(canvas, isFocused);
        }

        maybeRenderDraggedLine(canvas);
    }

    void BlocksManager::maybeRenderDraggedLine(SkCanvas* canvas) const {
        if (lastMouseClickTime > 0) {
            SkPaint paint;
            paint.setColor(SK_ColorGREEN);
            canvas->drawLine(0, 0, mouseX, mouseY, paint);
        }
    }

}  // namespace gui::logic
