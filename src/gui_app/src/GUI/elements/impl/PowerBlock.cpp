#include "PowerBlock.h"

#include <cmath>

namespace gui::elements::impl {
    PowerBlock::PowerBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    POWER_BLOCK_WIDTH,
                    POWER_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void PowerBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void PowerBlock::calculateOutputValues() {
        portValues[&outputPorts[0]] =
            boost::multiprecision::pow(getPortValue(&inputPorts[0]), getPortValue(&inputPorts[1]));
    }

    const std::vector<gui::elements::base::Port> PowerBlock::inputPorts = {
        {.name = "base", .type = gui::elements::base::Port::Type::INPUT},
        {.name = "exponent", .type = gui::elements::base::Port::Type::INPUT},
    };

    const std::vector<gui::elements::base::Port> PowerBlock::outputPorts = {
        {.name = "power", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
