#include "MultiplyBlock.h"

namespace gui::elements::impl {
    MultiplyBlock::MultiplyBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    MULTIPLY_BLOCK_WIDTH,
                    MULTIPLY_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void MultiplyBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void MultiplyBlock::calculateOutputValues() {
        portValues[&outputPorts[0]] = getPortValue(&inputPorts[0]) * getPortValue(&inputPorts[1]);
    }

    const std::vector<gui::elements::base::Port> MultiplyBlock::inputPorts = {
        {.name = "multiplicand 1", .type = gui::elements::base::Port::Type::INPUT},
        {.name = "multiplicand 2", .type = gui::elements::base::Port::Type::INPUT},
    };

    const std::vector<gui::elements::base::Port> MultiplyBlock::outputPorts = {
        {.name = "product", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
