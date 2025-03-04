#include "SubtractBlock.h"

namespace gui::elements::impl {
    SubtractBlock::SubtractBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    SUBTRACT_BLOCK_WIDTH,
                    SUBTRACT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void SubtractBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void SubtractBlock::calculateOutputValues() {
        portValues[&outputPorts[0]] = getPortValue(&inputPorts[0]) - getPortValue(&inputPorts[1]);
    }

    const std::vector<gui::elements::base::Port> SubtractBlock::inputPorts = {
        {.name = "minuend", .type = gui::elements::base::Port::Type::INPUT},
        {.name = "subtrahend", .type = gui::elements::base::Port::Type::INPUT},
    };

    const std::vector<gui::elements::base::Port> SubtractBlock::outputPorts = {
        {.name = "difference", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
