#include "InvertBlock.h"

namespace gui::elements::impl {
    InvertBlock::InvertBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    INVERT_BLOCK_WIDTH,
                    INVERT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void InvertBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void InvertBlock::calculateOutputValues() {
        static const FloatingPoint one("1");

        portValues[&outputPorts[0]] = one / getPortValue(&inputPorts[0]);
    }

    const std::vector<gui::elements::base::Port> InvertBlock::inputPorts = {
        {.name = "value", .type = gui::elements::base::Port::Type::INPUT},
    };

    const std::vector<gui::elements::base::Port> InvertBlock::outputPorts = {
        {.name = "inverse", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
