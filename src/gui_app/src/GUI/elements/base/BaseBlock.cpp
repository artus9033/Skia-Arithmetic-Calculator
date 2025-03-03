#include "BaseBlock.h"

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

    void BaseBlock::render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) {
        // dark gray outline
        static SkPaint blockOutlinePaint = []() {
            SkPaint paint;

            paint.setColor(colors::BLOCK_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // red hovered outline
        static SkPaint blockOutlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(colors::BLOCK_HOVERED_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(BLOCK_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // blueish hovered outline
        static SkPaint portOutlineHoveredPaint = []() {
            SkPaint paint;

            paint.setColor(colors::PURPLE_BLUE);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        static SkPaint portOutlinePaint = []() {
            SkPaint paint;

            paint.setColor(colors::BLOCK_PORT_OUTLINE_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setStrokeWidth(PORT_CIRCLE_OUTLINE_WIDTH);
            paint.setAntiAlias(true);

            return paint;
        }();

        // light blue gray fill
        static SkPaint blockFillPaint = []() {
            SkPaint paint;

            paint.setColor(colors::BLOCK_BACKGROUND_COLOR);
            paint.setStyle(SkPaint::Style::kFill_Style);

            return paint;
        }();

        static SkPaint textPaint = []() {
            SkPaint paint;

            paint.setColor(colors::TEXT_COLOR);
            paint.setStyle(SkPaint::Style::kStroke_Style);
            paint.setAntiAlias(true);

            return paint;
        }();

        auto& inputPorts = getInputPorts();
        auto& outputPorts = getOutputPorts();

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
                         isHovered ? blockOutlineHoveredPaint : blockOutlinePaint);
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY), blockFillPaint);

        auto& captionFont =
            components::UIText::getFontForVariant(components::UIText::Variant::Caption);
        auto captionFontSize = captionFont.getSize();

        // draw the input ports
        for (size_t i = 0; i < inputPorts.size(); i++) {
            bool isPortHovered =
                isHovered && gui::geometry::isCircleHovered(
                                 mouseX, mouseY, inputCx, inputCy, TOTAL_PORT_HITBOX_RADIUS);

            const SkPaint& renderOutlinePaint =
                isPortHovered ? portOutlineHoveredPaint : portOutlinePaint;

            canvas->drawCircle(inputCx, inputCy, PORT_CIRCLE_RADIUS, renderOutlinePaint);
            canvas->drawCircle(inputCx, inputCy, PORT_CIRCLE_RADIUS, blockFillPaint);

            if (isPortHovered) {
                auto cstr = inputPorts[i].name.c_str();

                // measure the text
                SkScalar width = captionFont.measureText(cstr, strlen(cstr), SkTextEncoding::kUTF8);

                // render port name to the left of the port
                canvas->drawString(cstr,
                                   inputCx - TOTAL_PORT_RADIUS_HALF - captionFontSize - width,
                                   inputCy + TOTAL_PORT_RADIUS_HALF - captionFontSize / 2,
                                   captionFont,
                                   textPaint);
            }

            // update the port coordinates map
            portCoordinates[&inputPorts[i]] = {.x = inputCx, .y = inputCy};

            inputCy += TOTAL_PORT_RADIUS * 2 + PORT_CIRCLE_MARGIN;
        }

        // draw the output ports
        for (size_t i = 0; i < outputPorts.size(); i++) {
            bool isPortHovered =
                isHovered && gui::geometry::isCircleHovered(
                                 mouseX, mouseY, outputCx, outputCy, TOTAL_PORT_HITBOX_RADIUS);

            const SkPaint& renderOutlinePaint =
                isPortHovered ? portOutlineHoveredPaint : portOutlinePaint;

            canvas->drawCircle(outputCx, outputCy, PORT_CIRCLE_RADIUS, renderOutlinePaint);
            canvas->drawCircle(outputCx, outputCy, PORT_CIRCLE_RADIUS, blockFillPaint);

            if (isPortHovered) {
                auto cstr = outputPorts[i].name.c_str();

                // render port name to the right of the port
                canvas->drawString(cstr,
                                   outputCx + TOTAL_PORT_RADIUS_HALF + captionFontSize,
                                   outputCy + TOTAL_PORT_RADIUS_HALF - captionFontSize / 2,
                                   captionFont,
                                   textPaint);
            }

            // update the port coordinates map
            portCoordinates[&outputPorts[i]] = {.x = outputCx, .y = outputCy};

            outputCy += TOTAL_PORT_RADIUS * 2 + PORT_CIRCLE_MARGIN;
        }
    }

    gui::geometry::Point2D BaseBlock::getPortCoordinates(
        const gui::elements::base::Port* port) const {
        if (portCoordinates.find(port) != portCoordinates.end()) {
            return portCoordinates.at(port);
        }

        return {.x = 0, .y = 0};
    }

    std::optional<const gui::elements::base::Port*> BaseBlock::checkPort(
        const gui::elements::base::Port* port, const geometry::Point2D& point) const {
        auto predCoordinates = getPortCoordinates(port);

        if (gui::geometry::isCircleHovered(
                point.x, point.y, predCoordinates.x, predCoordinates.y, TOTAL_PORT_HITBOX_RADIUS)) {
            return std::make_optional(port);
        }

        return std::nullopt;
    }

    std::optional<const gui::elements::base::Port*> BaseBlock::getPortAtCoordinates(
        const geometry::Point2D& point) const {
        for (const auto& port : getInputPorts()) {
            if (auto maybePort = checkPort(&port, point)) {
                return maybePort;
            }
        }

        for (const auto& port : getOutputPorts()) {
            if (auto maybePort = checkPort(&port, point)) {
                return maybePort;
            }
        }

        // port not found
        return std::nullopt;
    }
}  // namespace gui::elements::base
