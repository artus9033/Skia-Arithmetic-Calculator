#include "ConstantBlock.h"

namespace gui::elements::impl {
    ConstantBlock::ConstantBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        const geometry::Size2D& windowSize)
        : BaseBlock(cx,
                    cy,
                    CONSTANT_BLOCK_WIDTH,
                    CONSTANT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    logger,
                    windowSize),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void ConstantBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void ConstantBlock::onDoubleClick(int x, int y) {
        logger->info("Double clicked at ({}, {}), opening editor", x, y);

        // TODO: open editor
    }

    const std::vector<gui::elements::base::Port> ConstantBlock::inputPorts = {
        {.name = "in", .type = gui::elements::base::Port::Type::INPUT},
    };
    const std::vector<gui::elements::base::Port> ConstantBlock::outputPorts = {
        {.name = "value", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
