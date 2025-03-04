#include "MonitorBlock.h"

namespace gui::elements::impl {
    MonitorBlock::MonitorBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    MONITOR_BLOCK_WIDTH,
                    MONITOR_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    std::optional<std::string> MonitorBlock::getValueToRenderAboveBlock(bool isHovered) {
        const gui::elements::base::Port* theInputPort = &inputPorts[0];
        const gui::logic::PortsConnectionSide theInputSide{.block = this, .port = theInputPort};

        // draw the input port value above the block (if the input is connected)
        if (blockLifecycleManagerDelegate->isInputConnected(theInputSide)) {
            auto blockValue = getPortValue(theInputPort);

            std::ostringstream oss;
            // display either up to constants::defaultValueDisplayPrecision significant digits or up
            // to the max floating point structure capability (if hovered), in scientific notation
            oss << std::scientific
                << std::setprecision(isHovered ? std::numeric_limits<FloatingPoint>::digits10
                                               : constants::defaultValueDisplayPrecision)
                << blockValue;

            return oss.str();
        }

        return std::nullopt;
    }

    const std::vector<gui::elements::base::Port> MonitorBlock::inputPorts = {
        {.name = "in", .type = gui::elements::base::Port::Type::INPUT},
    };
    const std::vector<gui::elements::base::Port> MonitorBlock::outputPorts = {};

}  // namespace gui::elements::impl
