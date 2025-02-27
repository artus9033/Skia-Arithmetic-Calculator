#include "ConstantBlock.h"

namespace gui::elements::impl {
    ConstantBlock::ConstantBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        const geometry::Size2D& windowSize)
        : BaseBlock(cx, cy, CONSTANT_BLOCK_WIDTH, CONSTANT_BLOCK_HEIGHT, logger, windowSize),
          selfId(business_logic::stringifyAddressOf(this)),
          newBlockChoiceDelegate(newBlockChoiceDelegate) {}

    void ConstantBlock::render(SkCanvas* canvas, [[maybe_unused]] bool isHovered) const {
        SkPaint paint;
        paint.setColor(SK_ColorGREEN);
        canvas->drawRect(SkRect::MakeXYWH(x, y, width, height), paint);
    }

    void ConstantBlock::onDoubleClick(int x, int y) {
        logger->info("Double clicked at ({}, {}), opening editor", x, y);

        // TODO: open editor
    }

}  // namespace gui::elements::impl
