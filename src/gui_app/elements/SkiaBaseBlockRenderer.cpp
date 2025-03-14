#include "SkiaBaseBlockRenderer.h"

namespace gui::elements {
    // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    void SkiaBaseBlockRenderer::render(business_logic::elements::blocks::BaseBlock* block,
                                       SkCanvas* canvas,
                                       int mouseX,
                                       int mouseY,
                                       bool isHovered) {  // dark gray outline
        static SkPaint const blockOutlinePaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // red hovered outline
        static SkPaint const blockOutlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_HOVERED_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // blueish hovered outline
        static SkPaint const portOutlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::PURPLE_BLUE);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        static SkPaint const portOutlinePaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_PORT_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // light blue gray fill
        static SkPaint const blockFillPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::BLOCK_BACKGROUND_COLOR);
            paint.setStyle(SkPaint::Style::kFill_Style);

            return paint;
        }();

        static SkPaint const textPaint = []() {
            SkPaint paint;

            paint.setColor(gui::renderer::colors::TEXT_COLOR);
            paint.setStyle(SkPaint::Style::kFill_Style);
            paint.setAntiAlias(true);

            return paint;
        }();

        const auto& inputPorts = block->getInputPorts();
        const auto& outputPorts = block->getOutputPorts();

        const auto leftX = block->getLeftX();
        const auto rightX = block->getRightX();
        const auto topY = block->getTopY();
        const auto bottomY = block->getBottomY();
        const auto cy = block->getCy();
        const auto width = block->getWidth();
        auto& portCoordinates = block->getPortCoordinates();

        // note 1: the below uses int arithmetic for performance optimization; the assumption is
        // that the constants are for sure integers, which is guarded by static_assert in the header
        // ---
        // note 2: below a sum of port circle radius / 2  + port circle outline width is used
        // instead of TOTAL_PORT_RADIUS, since leftX is already at the edge of the block; therefore
        // the offset must be by half of the radius + the outline width
        const auto portCircleOffsetFromBlockEdge =
            business_logic::constants::PORT_CIRCLE_RADIUS_HALF +
            business_logic::constants::PORT_CIRCLE_OUTLINE_WIDTH;
        const auto inputCx = leftX + portCircleOffsetFromBlockEdge;
        const auto outputCx = rightX - portCircleOffsetFromBlockEdge;

        const auto inputsTotalHeight =
            business_logic::constants::TOTAL_PORT_RADIUS * 2 * inputPorts.size() +
            business_logic::constants::PORT_CIRCLE_MARGIN * (inputPorts.size() - 1);

        const auto outputsTotalHeight =
            business_logic::constants::TOTAL_PORT_RADIUS * 2 * outputPorts.size() +
            business_logic::constants::PORT_CIRCLE_MARGIN * (outputPorts.size() - 1);

        auto inputCy = static_cast<int>(std::round(cy - (inputsTotalHeight / 2.0F)));
        auto outputCy = static_cast<int>(std::round(cy - (outputsTotalHeight / 2.0F)));

        // draw the block
        // NOLINTNEXTLINE(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY),
                         isHovered ? blockOutlineHoveredPaint : blockOutlinePaint);
        // NOLINTNEXTLINE(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY), blockFillPaint);

        const auto captionFont =
            gui::renderer::FontManager::getFontForVariant(components::UIText::Variant::Caption);
        const auto captionFontSize = captionFont.getSize();

        const auto* blockNameCstr = magic_enum::enum_name(block->getBlockType()).data();
        auto blockNameWidth =
            captionFont.measureText(blockNameCstr, strlen(blockNameCstr), SkTextEncoding::kUTF8);

        // draw the block name
        canvas->drawString(blockNameCstr,
                           leftX + (static_cast<SkScalar>(width) / 2.0F) - (blockNameWidth / 2),
                           cy + (captionFontSize / 4),
                           captionFont,
                           textPaint);

        // draw the input ports
        for (size_t i = 0; i < inputPorts.size(); i++) {
            // advance to the center of the circle
            inputCy += business_logic::constants::TOTAL_PORT_RADIUS;

            // update the port coordinates map after advancing
            portCoordinates[&inputPorts[i]] = {.x = inputCx, .y = inputCy};

            // check if port hovered after advancing to the center
            bool const isPortHovered =
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
                const auto* cstr = inputPorts[i].name.c_str();

                // measure the text
                SkScalar const captionWidth =
                    captionFont.measureText(cstr, strlen(cstr), SkTextEncoding::kUTF8);

                // render port name to the left of the port
                canvas->drawString(cstr,
                                   inputCx - business_logic::constants::TOTAL_PORT_RADIUS_HALF -
                                       captionFontSize - captionWidth,
                                   inputCy + business_logic::constants::TOTAL_PORT_RADIUS_HALF -
                                       (captionFontSize / 4),
                                   captionFont,
                                   textPaint);
            }

            inputCy += business_logic::constants::TOTAL_PORT_RADIUS +
                       business_logic::constants::PORT_CIRCLE_MARGIN;
        }

        // draw the output ports
        for (size_t i = 0; i < outputPorts.size(); i++) {
            // advance to the center of the circle
            outputCy += business_logic::constants::TOTAL_PORT_RADIUS;

            // update the port coordinates map after advancing
            portCoordinates[&outputPorts[i]] = {.x = outputCx, .y = outputCy};

            // check if port hovered after advancing to the center
            bool const isPortHovered =
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
                const auto* cstr = outputPorts[i].name.c_str();

                // render port name to the right of the port
                canvas->drawString(
                    cstr,
                    outputCx + business_logic::constants::TOTAL_PORT_RADIUS_HALF + captionFontSize,
                    outputCy + business_logic::constants::TOTAL_PORT_RADIUS_HALF -
                        (captionFontSize / 4),
                    captionFont,
                    textPaint);
            }

            outputCy += business_logic::constants::TOTAL_PORT_RADIUS +
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

        const auto* blockValueCstr = blockValue.c_str();
        auto blockValueWidth = gui::renderer::FontManager::captionFont.measureText(
            blockValueCstr, strlen(blockValueCstr), SkTextEncoding::kUTF8);

        canvas->drawString(blockValueCstr,
                           leftX + (static_cast<SkScalar>(width) / 2.0F) - (blockValueWidth / 2.0F),
                           topY - gui::renderer::FontManager::captionFont.getSize(),
                           gui::renderer::FontManager::captionFont,
                           gui::renderer::FontManager::textFontFillPaint);
    }
    // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
}  // namespace gui::elements
