#include "AddBlock.h"

namespace gui::elements::impl {
    AddBlock::AddBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    ADD_BLOCK_WIDTH,
                    ADD_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void AddBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);
    }

    void AddBlock::calculateOutputValues() {
        portValues[&outputPorts[0]] = getPortValue(&inputPorts[0]) + getPortValue(&inputPorts[1]);
    }

    const std::vector<gui::elements::base::Port> AddBlock::inputPorts = {
        {.name = "addend 1", .type = gui::elements::base::Port::Type::INPUT},
        {.name = "addend 2", .type = gui::elements::base::Port::Type::INPUT},
    };

    const std::vector<gui::elements::base::Port> AddBlock::outputPorts = {
        {.name = "sum", .type = gui::elements::base::Port::Type::OUTPUT},
    };

}  // namespace gui::elements::impl
