#include "MonitorBlock.h"

namespace business_logic::elements::blocks {

    MonitorBlock::MonitorBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : business_logic::Loggable<MonitorBlock>(),
          BaseBlock(cx,
                    cy,
                    MONITOR_BLOCK_WIDTH,
                    MONITOR_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    std::optional<std::string> MonitorBlock::getValueToRenderAboveBlock(bool isHovered) {
        const business_logic::elements::structures::Port* theInputPort = &inputPorts[0];
        const business_logic::elements::structures::BlocksConnectionSide theInputSide{
            .block = this, .port = theInputPort};

        // draw the input port value above the block (if the input is connected)
        if (this->blockLifecycleManagerDelegate->isInputConnected(theInputSide)) {
            auto blockValue = this->getPortValue(theInputPort);

            std::ostringstream oss;
            // display either up to business_logic::constants::DEFAULT_VALUE_DISPLAY_PRECISION
            // significant digits or up to the max floating point structure capability (if hovered),
            // in scientific notation
            oss << std::scientific
                << std::setprecision(
                       isHovered ? std::numeric_limits<FloatingPoint>::digits10
                                 : business_logic::constants::DEFAULT_VALUE_DISPLAY_PRECISION)
                << blockValue;

            return oss.str();
        }

        return std::nullopt;
    }

    const std::vector<business_logic::elements::structures::Port> MonitorBlock::inputPorts = {
        {.name = "in", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> MonitorBlock::outputPorts = {};

}  // namespace business_logic::elements::blocks
