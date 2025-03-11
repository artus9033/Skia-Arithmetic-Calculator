#include "AddBlock.h"

namespace business_logic::elements::blocks {

    AddBlock::AddBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : business_logic::Loggable<AddBlock>(),
          BaseBlock(cx,
                    cy,
                    ADD_BLOCK_WIDTH,
                    ADD_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void AddBlock::calculateOutputValues() {
        this->portValues[&outputPorts[0]] =
            this->getPortValue(&inputPorts[0]) + this->getPortValue(&inputPorts[1]);
    }

    const std::vector<business_logic::elements::structures::Port> AddBlock::inputPorts = {
        {.name = "addend 1", .type = business_logic::elements::structures::Port::Type::INPUT},
        {.name = "addend 2", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> AddBlock::outputPorts = {
        {.name = "sum", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
