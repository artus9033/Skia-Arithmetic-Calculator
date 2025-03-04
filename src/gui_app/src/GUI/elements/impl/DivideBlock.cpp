#include "DivideBlock.h"

namespace gui::elements::impl {
    DivideBlock::DivideBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    DIVIDE_BLOCK_WIDTH,
                    DIVIDE_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void DivideBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void DivideBlock::calculateOutputValues() {
        portValues[&outputPorts[0]] = getPortValue(&inputPorts[0]) / getPortValue(&inputPorts[1]);
    }

    const std::vector<gui::elements::base::Port> DivideBlock::inputPorts = {
        {.name = "numerator", .type = gui::elements::base::Port::Type::INPUT},
        {.name = "denominator", .type = gui::elements::base::Port::Type::INPUT},
    };

    const std::vector<gui::elements::base::Port> DivideBlock::outputPorts = {
        {.name = "quotient", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
