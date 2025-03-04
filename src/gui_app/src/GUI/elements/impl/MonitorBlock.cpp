#include "MonitorBlock.h"

namespace gui::elements::impl {
    MonitorBlock::MonitorBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        gui::window::delegate::IWindowDelegate* windowDelegate)
        : BaseBlock(cx,
                    cy,
                    MONITOR_BLOCK_WIDTH,
                    MONITOR_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    windowDelegate,
                    logger),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void MonitorBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        const gui::elements::base::Port* theInputPort = &inputPorts[0];
        const gui::logic::PortsConnectionSide theInputSide{.block = this, .port = theInputPort};

        BaseBlock::render(canvas, mouseX, mouseY, isHovered);

        // draw the input port value above the block (if the input is connected)
        if (blockLifecycleManagerDelegate->isInputConnected(theInputSide)) {
            auto blockValue = getPortValue(theInputPort);

            std::ostringstream oss;
            oss << std::scientific << std::setprecision(10) << blockValue;
            auto blockValueStr = oss.str();
            auto blockValueCstr = blockValueStr.c_str();
            auto blockValueWidth = gui::renderer::FontManager::captionFont.measureText(
                blockValueCstr, strlen(blockValueCstr), SkTextEncoding::kUTF8);

            canvas->drawString(blockValueCstr,
                               leftX + width / 2 - blockValueWidth / 2,
                               topY - gui::renderer::FontManager::captionFont.getSize(),
                               gui::renderer::FontManager::captionFont,
                               gui::renderer::FontManager::textFontFillPaint);
        }
    }

    const std::vector<gui::elements::base::Port> MonitorBlock::inputPorts = {
        {.name = "in", .type = gui::elements::base::Port::Type::INPUT},
    };
    const std::vector<gui::elements::base::Port> MonitorBlock::outputPorts = {};

}  // namespace gui::elements::impl
