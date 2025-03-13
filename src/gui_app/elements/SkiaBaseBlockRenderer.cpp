#include "SkiaBaseBlockRenderer.h"

namespace gui::elements {

    void SkiaBaseBlockRenderer::render(business_logic::elements::blocks::BaseBlock* block,
                                       SkCanvas* canvas,
                                       int mouseX,
                                       int mouseY,
                                       bool isHovered) {  // dark gray outline
        static SkPaint blockOutlinePaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // red hovered outline
        static SkPaint blockOutlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_HOVERED_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // blueish hovered outline
        static SkPaint portOutlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::PURPLE_BLUE);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        static SkPaint portOutlinePaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_PORT_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // light blue gray fill
        static SkPaint blockFillPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_BACKGROUND_COLOR);
            paint.setStyle(SkPaint::Style::kFill_Style);

            return paint;
        }();

        static SkPaint textPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::TEXT_COLOR);
            paint.setStyle(SkPaint::Style::kFill_Style);
            paint.setAntiAlias(true);

            return paint;
        }();

        auto& inputPorts = block->getInputPorts();
        auto& outputPorts = block->getOutputPorts();

        const auto leftX = block->getLeftX();
        const auto rightX = block->getRightX();
        const auto topY = block->getTopY();
        const auto bottomY = block->getBottomY();
        const auto cy = block->getCy();
        const auto width = block->getWidth();
        auto& portCoordinates = block->getPortCoordinates();

        // note: the below uses int arithmetic for performance optimization; the assumption is that
        // business_logic::constants::PORT_CIRCLE_RADIUS is an int already, which is guarded by
        // static_assert in the header
        int inputCx = leftX + business_logic::constants::TOTAL_PORT_RADIUS_HALF,
            outputCx = rightX - business_logic::constants::TOTAL_PORT_RADIUS_HALF,
            inputCy = cy -
                      (business_logic::constants::PORT_CIRCLE_RADIUS_HALF -
                       business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH / 2) *
                          inputPorts.size() -
                      business_logic::constants::PORT_CIRCLE_MARGIN_HALF * (inputPorts.size() - 1),
            outputCy =
                cy -
                (business_logic::constants::PORT_CIRCLE_RADIUS_HALF -
                 business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH / 2) *
                    outputPorts.size() -
                business_logic::constants::PORT_CIRCLE_MARGIN_HALF * (outputPorts.size() - 1);

        // draw the block
        // NOLINTNEXTLINE(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY),
                         isHovered ? blockOutlineHoveredPaint : blockOutlinePaint);
        // NOLINTNEXTLINE(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY), blockFillPaint);

        const auto captionFont =
            gui::renderer::FontManager::getFontForVariant(components::UIText::Variant::Caption);
        const auto captionFontSize = captionFont.getSize();

        auto blockNameCstr = magic_enum::enum_name(block->getBlockType()).data();
        auto blockNameWidth =
            captionFont.measureText(blockNameCstr, strlen(blockNameCstr), SkTextEncoding::kUTF8);

        // draw the block name
        canvas->drawString(blockNameCstr,
                           leftX + width / 2 - blockNameWidth / 2,
                           cy + captionFontSize / 4,
                           captionFont,
                           textPaint);

        // draw the input ports
        for (size_t i = 0; i < inputPorts.size(); i++) {
            bool isPortHovered =
                isHovered && business_logic::geometry::isCircleHovered(
                                 mouseX,
                                 mouseY,
                                 inputCx,
                                 inputCy,
                                 business_logic::constants::TOTAL_PORT_HITBOX_RADIUS);

            const SkPaint& renderOutlinePaint =
                isPortHovered ? portOutlineHoveredPaint : portOutlinePaint;

            canvas->drawCircle(inputCx,
                               inputCy,
                               business_logic::constants::PORT_CIRCLE_RADIUS,
                               renderOutlinePaint);
            canvas->drawCircle(
                inputCx, inputCy, business_logic::constants::PORT_CIRCLE_RADIUS, blockFillPaint);

            if (isPortHovered) {
                auto cstr = inputPorts[i].name.c_str();

                // measure the text
                SkScalar captionWidth =
                    captionFont.measureText(cstr, strlen(cstr), SkTextEncoding::kUTF8);

                // render port name to the left of the port
                canvas->drawString(cstr,
                                   inputCx - business_logic::constants::TOTAL_PORT_RADIUS_HALF -
                                       captionFontSize - captionWidth,
                                   inputCy + business_logic::constants::TOTAL_PORT_RADIUS_HALF -
                                       captionFontSize / 4,
                                   captionFont,
                                   textPaint);
            }

            // update the port coordinates map
            portCoordinates[&inputPorts[i]] = {.x = inputCx, .y = inputCy};

            inputCy += business_logic::constants::TOTAL_PORT_RADIUS * 2 +
                       business_logic::constants::PORT_CIRCLE_MARGIN;
        }

        // draw the output ports
        for (size_t i = 0; i < outputPorts.size(); i++) {
            bool isPortHovered =
                isHovered && business_logic::geometry::isCircleHovered(
                                 mouseX,
                                 mouseY,
                                 outputCx,
                                 outputCy,
                                 business_logic::constants::TOTAL_PORT_HITBOX_RADIUS);

            const SkPaint& renderOutlinePaint =
                isPortHovered ? portOutlineHoveredPaint : portOutlinePaint;

            canvas->drawCircle(outputCx,
                               outputCy,
                               business_logic::constants::PORT_CIRCLE_RADIUS,
                               renderOutlinePaint);
            canvas->drawCircle(
                outputCx, outputCy, business_logic::constants::PORT_CIRCLE_RADIUS, blockFillPaint);

            if (isPortHovered) {
                auto cstr = outputPorts[i].name.c_str();

                // render port name to the right of the port
                canvas->drawString(
                    cstr,
                    outputCx + business_logic::constants::TOTAL_PORT_RADIUS_HALF + captionFontSize,
                    outputCy + business_logic::constants::TOTAL_PORT_RADIUS_HALF -
                        captionFontSize / 4,
                    captionFont,
                    textPaint);
            }

            // update the port coordinates map
            portCoordinates[&outputPorts[i]] = {.x = outputCx, .y = outputCy};

            outputCy += business_logic::constants::TOTAL_PORT_RADIUS * 2 +
                        business_logic::constants::PORT_CIRCLE_MARGIN;
        }

        if (auto blockValue = block->getValueToRenderAboveBlock(isHovered)) {
            renderValueAboveBlock(block, canvas, isHovered, blockValue.value());
        }
    }

    void SkiaBaseBlockRenderer::renderValueAboveBlock(
        const business_logic::elements::blocks::BaseBlock* block,
        SkCanvas* canvas,
        [[maybe_unused]] bool isHovered,
        const std::string& blockValue) {
        const auto leftX = block->getLeftX();
        const auto topY = block->getTopY();
        const auto width = block->getWidth();

        auto blockValueCstr = blockValue.c_str();
        auto blockValueWidth = gui::renderer::FontManager::captionFont.measureText(
            blockValueCstr, strlen(blockValueCstr), SkTextEncoding::kUTF8);

        canvas->drawString(blockValueCstr,
                           leftX + width / 2 - blockValueWidth / 2,
                           topY - gui::renderer::FontManager::captionFont.getSize(),
                           gui::renderer::FontManager::captionFont,
                           gui::renderer::FontManager::textFontFillPaint);
    }
}  // namespace gui::elements
