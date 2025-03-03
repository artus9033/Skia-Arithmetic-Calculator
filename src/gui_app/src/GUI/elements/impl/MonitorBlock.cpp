#include "MonitorBlock.h"

namespace gui::elements::impl {
    MonitorBlock::MonitorBlock(
        int cx,
        int cy,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        const geometry::Size2D& windowSize)
        : BaseBlock(cx,
                    cy,
                    MONITOR_BLOCK_WIDTH,
                    MONITOR_BLOCK_HEIGHT,
                    newBlockChoiceDelegate,
                    blockLifecycleManagerDelegate,
                    logger,
                    windowSize),
          selfId(business_logic::stringifyAddressOf(this)) {}

    void MonitorBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        BaseBlock::render(canvas, mouseX, mouseY, isHovered);

        // draw the input port value above the block
        auto blockNameCstr = gui::elements::base::BlockTypeNames.at(getBlockType()).c_str();
        auto blockNameWidth = gui::renderer::FontManager::captionFont.measureText(
            blockNameCstr, strlen(blockNameCstr), SkTextEncoding::kUTF8);

        // draw the block name
        canvas->drawString(blockNameCstr,
                           leftX + width / 2 - blockNameWidth / 2,
                           centerY + gui::renderer::FontManager::captionFont.getSize() / 4,
                           gui::renderer::FontManager::captionFont,
                           gui::renderer::FontManager::textFontFillPaint);
    }

    const std::vector<gui::elements::base::Port> MonitorBlock::inputPorts = {
        {.name = "in", .type = gui::elements::base::Port::Type::INPUT},
    };
    const std::vector<gui::elements::base::Port> MonitorBlock::outputPorts = {};

}  // namespace gui::elements::impl
