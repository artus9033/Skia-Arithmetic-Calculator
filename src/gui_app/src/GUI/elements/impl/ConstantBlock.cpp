#include "ConstantBlock.h"

namespace gui::elements::impl {
    ConstantBlock::ConstantBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    CONSTANT_BLOCK_WIDTH,
                    CONSTANT_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void ConstantBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void ConstantBlock::onDoubleClick(int x, int y) {
        logger->info("Double clicked at ({}, {}), opening editor", x, y);

        auto maybeNewValue = gui::window::prompt::TextInputDialog::promptForFloatingPointInput(
            "Edit Constant Value", "Value:", portValues[&outputPorts[0]], windowDelegate);

        if (maybeNewValue) {
            auto newValue = maybeNewValue.value();

            logger->info("Constant block {} value changed from '{}' to '{}'",
                         selfId,
                         portValues[&outputPorts[0]].str(),
                         newValue.str());

            // update the output port value
            portValues[&outputPorts[0]] = newValue;
        } else {
            gui::window::prompt::MessageBox::showWarning(
                "Invalid number", "The entered value is not a valid number", windowDelegate);
        }
    }

    const std::vector<gui::elements::base::Port> ConstantBlock::inputPorts = {};
    const std::vector<gui::elements::base::Port> ConstantBlock::outputPorts = {
        {.name = "value", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
