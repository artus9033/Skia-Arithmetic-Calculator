#include "DivideBlock.h"

namespace business_logic::elements::blocks {

    DivideBlock::DivideBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : business_logic::Loggable<DivideBlock>(),
          BaseBlock(cx,
                    cy,
                    DIVIDE_BLOCK_WIDTH,
                    DIVIDE_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void DivideBlock::calculateOutputValues() {
        this->portValues[&outputPorts[0]] =
            this->getPortValue(&inputPorts[0]) / this->getPortValue(&inputPorts[1]);
    }

    const std::vector<business_logic::elements::structures::Port> DivideBlock::inputPorts = {
        {.name = "numerator", .type = business_logic::elements::structures::Port::Type::INPUT},
        {.name = "denominator", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> DivideBlock::outputPorts = {
        {.name = "quotient", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
