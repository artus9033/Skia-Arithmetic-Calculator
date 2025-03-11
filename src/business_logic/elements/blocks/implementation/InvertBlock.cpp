#include "InvertBlock.h"

namespace business_logic::elements::blocks {

    InvertBlock::InvertBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : business_logic::Loggable<InvertBlock>(),
          BaseBlock(cx,
                    cy,
                    INVERT_BLOCK_WIDTH,
                    INVERT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void InvertBlock::calculateOutputValues() {
        static const FloatingPoint one("1");

        this->portValues[&outputPorts[0]] = one / this->getPortValue(&inputPorts[0]);
    }

    const std::vector<business_logic::elements::structures::Port> InvertBlock::inputPorts = {
        {.name = "value", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> InvertBlock::outputPorts = {
        {.name = "inverse", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
