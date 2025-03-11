#include "SubtractBlock.h"

namespace business_logic::elements::blocks {

    SubtractBlock::SubtractBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : business_logic::Loggable<SubtractBlock>(),
          BaseBlock(cx,
                    cy,
                    SUBTRACT_BLOCK_WIDTH,
                    SUBTRACT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void SubtractBlock::calculateOutputValues() {
        this->portValues[&outputPorts[0]] =
            this->getPortValue(&inputPorts[0]) - this->getPortValue(&inputPorts[1]);
    }

    const std::vector<business_logic::elements::structures::Port> SubtractBlock::inputPorts = {
        {.name = "minuend", .type = business_logic::elements::structures::Port::Type::INPUT},
        {.name = "subtrahend", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> SubtractBlock::outputPorts = {
        {.name = "difference", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
