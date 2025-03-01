#include "BaseBlock.h"
#define CAPTION_FONT_SIZE 8
namespace gui::elements::base {

    BaseBlock::BaseBlock(int cx,
                         int cy,
                         int blockWidth,
                         int blockHeight,
                         std::shared_ptr<spdlog::logger> logger,
                         const geometry::Size2D& windowSize)
        : width(blockWidth), height(blockHeight), windowSize(windowSize), logger(logger) {
        this->cx = cx;
        this->cy = cy;

        cacheCornerCoordinates();
    }

    void BaseBlock::updateWidthHeight(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;

        cacheCornerCoordinates();
    }

    void BaseBlock::onDragProgress(int x, int y) {
        logger->info("Block {} has been dragged", getSelfId());

        this->cx = x;
        this->cy = y;
    }

    void BaseBlock::onDragStart(int x, int y) {
        logger->info("Block {} has been dragged", getSelfId());

        this->cx = x;
        this->cy = y;
    }

    void BaseBlock::onDragEnd(int x, int y) {
        logger->info("Block {} has been destroyed", getSelfId());

        this->cx = x;
        this->cy = y;
    }

    void BaseBlock::cacheCornerCoordinates() {
        leftX = std::min(std::max(0, cx - width / 2), windowSize.width - width);
        topY = std::min(std::max(0, cy - height / 2), windowSize.height - height);

        bottomY = topY + height;

        rightX = leftX + width;

        if (rightX > windowSize.width) {
            leftX = windowSize.width - width;
            rightX = windowSize.width;
        }

        if (bottomY > windowSize.height) {
            topY = windowSize.height - height;
            bottomY = windowSize.height;
        }

        centerY = (topY + bottomY) / 2.0f;
    }

    void BaseBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) const {
        // dark blue grey outline
        static SkPaint outlinePaint = []() {
            SkPaint paint;

            paint.setColor(SkColorSetARGB(255, 100, 100, 100));
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // dark blue grey outline
        static SkPaint outlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(SK_ColorRED);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // blue-purple outline
        static SkPaint outlineHoveredPortPaint = []() {
            SkPaint paint;

            paint.setColor(SkColorSetARGB(255, 100, 100, 255));
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        static SkPaint outlinePortPaint = []() {
            SkPaint paint;

            paint.setColor(SkColorSetARGB(255, 100, 100, 100));
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // light blue grey fill
        static SkPaint fillPaint = []() {
            SkPaint paint;

            paint.setColor(SkColorSetARGB(255, 50, 50, 50));
            paint.setStyle(SkPaint::Style::kFill_Style);

            return paint;
        }();

        static SkPaint textPaint = []() {
            SkPaint paint;

            paint.setColor(SK_ColorWHITE);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setAntiAlias(true);

            return paint;
        }();

        static sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_CoreText(nullptr);
        static auto typeface = fontMgr->legacyMakeTypeface(nullptr, SkFontStyle::Normal());

        static SkFont captionFont = []() {
            SkFont font;

            font.setSize(CAPTION_FONT_SIZE);
            font.setTypeface(typeface);
            font.setEmbolden(true);

            return font;
        }();

        auto inputPorts = getInputPorts(), outputPorts = getOutputPorts();

        // note: the below uses int arithmetic for performance optimization; the assumption is that
        // PORT_CIRCLE_RADIUS is an int already, which is guarded by static_assert in the header
        int inputCx = leftX + TOTAL_PORT_RADIUS_HALF, outputCx = rightX - TOTAL_PORT_RADIUS_HALF,
            inputCy =
                centerY -
                (PORT_CIRCLE_RADIUS_HALF + PORT_CIRCLE_OUTLINE_WIDTH / 2) * inputPorts.size() -
                PORT_CIRCLE_MARGIN_HALF * (inputPorts.size() - 1),
            outputCy =
                centerY -
                (PORT_CIRCLE_RADIUS_HALF + PORT_CIRCLE_OUTLINE_WIDTH / 2) * outputPorts.size() -
                PORT_CIRCLE_MARGIN_HALF * (outputPorts.size() - 1);

        // draw the block
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY),
                         isHovered ? outlineHoveredPaint : outlinePaint);
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY), fillPaint);

        // draw the input ports
        for (size_t i = 0; i < inputPorts.size(); i++) {
            bool isPortHovered =
                isHovered && gui::geometry::isCircleHovered(
                                 mouseX, mouseY, inputCx, inputCy, TOTAL_PORT_HITBOX_RADIUS);

            const SkPaint& renderOutlinePaint =
                isPortHovered ? outlineHoveredPortPaint : outlinePortPaint;

            canvas->drawCircle(inputCx, inputCy, PORT_CIRCLE_RADIUS, renderOutlinePaint);
            canvas->drawCircle(inputCx, inputCy, PORT_CIRCLE_RADIUS, fillPaint);

            if (isPortHovered) {
                auto cstr = inputPorts[i].name.c_str();

                // measure the text
                SkScalar width = captionFont.measureText(cstr, strlen(cstr), SkTextEncoding::kUTF8);

                // render port name to the left of the port
                canvas->drawString(cstr,
                                   inputCx - TOTAL_PORT_RADIUS_HALF - CAPTION_FONT_SIZE - width,
                                   inputCy + TOTAL_PORT_RADIUS_HALF - CAPTION_FONT_SIZE / 2,
                                   captionFont,
                                   textPaint);
            }

            inputCy += TOTAL_PORT_RADIUS * 2 + PORT_CIRCLE_MARGIN;
        }

        // draw the output ports
        for (size_t i = 0; i < outputPorts.size(); i++) {
            bool isPortHovered =
                isHovered && gui::geometry::isCircleHovered(
                                 mouseX, mouseY, outputCx, outputCy, TOTAL_PORT_HITBOX_RADIUS);

            const SkPaint& renderOutlinePaint =
                isPortHovered ? outlineHoveredPortPaint : outlinePortPaint;

            canvas->drawCircle(outputCx, outputCy, PORT_CIRCLE_RADIUS, renderOutlinePaint);
            canvas->drawCircle(outputCx, outputCy, PORT_CIRCLE_RADIUS, fillPaint);

            if (isPortHovered) {
                auto cstr = outputPorts[i].name.c_str();

                // measure the text
                SkScalar width = captionFont.measureText(cstr, strlen(cstr), SkTextEncoding::kUTF8);

                // render port name to the right of the port
                canvas->drawString(cstr,
                                   outputCx + TOTAL_PORT_RADIUS_HALF + CAPTION_FONT_SIZE,
                                   outputCy + TOTAL_PORT_RADIUS_HALF - CAPTION_FONT_SIZE / 2,
                                   captionFont,
                                   textPaint);
            }

            outputCy += TOTAL_PORT_RADIUS * 2 + PORT_CIRCLE_MARGIN;
        }
    }

}  // namespace gui::elements::base
