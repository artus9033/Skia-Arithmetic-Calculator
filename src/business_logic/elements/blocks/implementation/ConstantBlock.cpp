#include "ConstantBlock.h"

namespace business_logic::elements::blocks {

    ConstantBlock::ConstantBlock(
        int cx,
        int cy,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    CONSTANT_BLOCK_WIDTH,
                    CONSTANT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void ConstantBlock::onDoubleClick(int x, int y) {
        logger->info("Double clicked at ({}, {}), opening editor", x, y);

        auto maybeNewValue = this->windowDelegate->promptFloatingPointInput(
            "Edit Constant Value", "Value:", this->portValues[&outputPorts[0]]);

        if (maybeNewValue) {
            const auto& newValue = maybeNewValue.value();

            logger->info("Constant block {} value changed from '{}' to '{}'",
                         selfId,
                         this->portValues[&outputPorts[0]].str(),
                         newValue.str());

            // update the output port value
            this->portValues[&outputPorts[0]] = newValue;
        } else {
            this->windowDelegate->showWarning("Invalid number",
                                              "The entered value is not a valid number");
        }
    }

    const std::vector<business_logic::elements::structures::Port> ConstantBlock::inputPorts = {};

    const std::vector<business_logic::elements::structures::Port> ConstantBlock::outputPorts = {
        {.name = "value", .type = business_logic::elements::structures::Port::Type::OUTPUT},
    };

}  // namespace business_logic::elements::blocks
