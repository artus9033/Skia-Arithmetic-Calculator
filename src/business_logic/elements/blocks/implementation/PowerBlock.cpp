#include "PowerBlock.h"

#include <cmath>

namespace business_logic::elements::blocks {

    PowerBlock::PowerBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    POWER_BLOCK_WIDTH,
                    POWER_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void PowerBlock::calculateOutputValues() {
        // NOLINTNEXTLINE
        this->portValues[&outputPorts[0]] = boost::multiprecision::pow(
            this->getPortValue(&inputPorts[0]), this->getPortValue(&inputPorts[1]));
    }

    const std::vector<business_logic::elements::structures::Port> PowerBlock::inputPorts = {
        {.name = "base", .type = business_logic::elements::structures::Port::Type::INPUT},
        {.name = "exponent", .type = business_logic::elements::structures::Port::Type::INPUT},
    };

    const std::vector<business_logic::elements::structures::Port> PowerBlock::outputPorts = {
        {.name = "power", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
