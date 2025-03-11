#include "MultiplyBlock.h"

namespace business_logic::elements::blocks {

    MultiplyBlock::MultiplyBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : business_logic::Loggable<MultiplyBlock>(),
          BaseBlock(cx,
                    cy,
                    MULTIPLY_BLOCK_WIDTH,
                    MULTIPLY_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void MultiplyBlock::calculateOutputValues() {
        this->portValues[&outputPorts[0]] =
            this->getPortValue(&inputPorts[0]) * this->getPortValue(&inputPorts[1]);
    }

    const std::vector<business_logic::elements::structures::Port> MultiplyBlock::inputPorts = {
        {.name = "multiplicand 1", .type = business_logic::elements::structures::Port::Type::INPUT},
        {.name = "multiplicand 2", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> MultiplyBlock::outputPorts = {
        {.name = "product", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
