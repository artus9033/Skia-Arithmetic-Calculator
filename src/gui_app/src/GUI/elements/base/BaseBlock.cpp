#include "BaseBlock.h"

namespace gui::elements::base {

    BaseBlock::BaseBlock(
        int cx,
        int cy,
        int blockWidth,
        int blockHeight,
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        std::shared_ptr<spdlog::logger> logger,
        const geometry::Size2D& windowSize)
        : width(blockWidth),
          height(blockHeight),
          windowSize(windowSize),
          logger(logger),
          newBlockChoiceDelegate(newBlockChoiceDelegate),
          blockLifecycleManagerDelegate(blockLifecycleManagerDelegate) {
        this->cx = cx;
        this->cy = cy;

        cacheCornerCoordinates();
    }

    BaseBlock::~BaseBlock() { blockLifecycleManagerDelegate->onBlockDeleted(this); }

    void BaseBlock::updateWidthHeight(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;

        cacheCornerCoordinates();
    }

    void BaseBlock::onDragProgress(int x, int y) {
        logger->info("Block {} drag progress to {}, {}", getSelfId(), x, y);

        this->cx = x;
        this->cy = y;

        cacheCornerCoordinates();
    }

    void BaseBlock::onDragStart() { logger->info("Block {} drag start", getSelfId()); }

    void BaseBlock::onDragEnd() { logger->info("Block {} drag end", getSelfId()); }

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

        centerY = static_cast<float>(topY + bottomY) / 2.0f;
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
            paint.setStyle(SkPaint::Style::kFill_Style);
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
                (PORT_CIRCLE_RADIUS_HALF - PORT_CIRCLE_OUTLINE_WIDTH / 2) * inputPorts.size() -
                PORT_CIRCLE_MARGIN_HALF * (inputPorts.size() - 1),
            outputCy =
                centerY -
                (PORT_CIRCLE_RADIUS_HALF - PORT_CIRCLE_OUTLINE_WIDTH / 2) * outputPorts.size() -
                PORT_CIRCLE_MARGIN_HALF * (outputPorts.size() - 1);

        // draw the block
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY),
                         isHovered ? blockOutlineHoveredPaint : blockOutlinePaint);
        canvas->drawRect(SkRect::MakeLTRB(leftX, topY, rightX, bottomY), blockFillPaint);

        auto& captionFont =
            components::UIText::getFontForVariant(components::UIText::Variant::Caption);
        auto captionFontSize = captionFont.getSize();

        auto blockNameCstr = BlockTypeNames.at(getBlockType()).c_str();
        auto blockNameWidth =
            captionFont.measureText(blockNameCstr, strlen(blockNameCstr), SkTextEncoding::kUTF8);

        // draw the block name
        canvas->drawString(blockNameCstr,
                           leftX + width / 2 - blockNameWidth / 2,
                           centerY + captionFontSize / 4,
                           captionFont,
                           textPaint);

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
                                   inputCy + TOTAL_PORT_RADIUS_HALF - captionFontSize / 4,
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
                                   outputCy + TOTAL_PORT_RADIUS_HALF - captionFontSize / 4,
                                   captionFont,
                                   textPaint);
            }

            // update the port coordinates map
            portCoordinates[&outputPorts[i]] = {.x = outputCx, .y = outputCy};

            outputCy += TOTAL_PORT_RADIUS * 2 + PORT_CIRCLE_MARGIN;
        }
    }

    const gui::geometry::Point2D& BaseBlock::getPortCoordinates(
        const gui::elements::base::Port* port) const {
        // trick to be able to return a static reference for this function; also optimizes the
        // function call
        static const geometry::Point2D zeroPoint = {.x = 0, .y = 0};

        if (portCoordinates.find(port) != portCoordinates.end()) {
            return portCoordinates.at(port);
        }

        return zeroPoint;
    }

    std::optional<const gui::elements::base::Port*> BaseBlock::checkPort(
        const gui::elements::base::Port* port, const geometry::Point2D& point) const {
        auto& predCoordinates = getPortCoordinates(port);

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

    const boost::multiprecision::cpp_dec_float_50& BaseBlock::getPortValue(
        const gui::elements::base::Port* port) const {
        // trick to be able to return a static reference for this function; also optimizes the
        // function call
        static const boost::multiprecision::cpp_dec_float_50 staticNaN =
            std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::quiet_NaN();

        auto maybeValueIt = portValues.find(port);

        if (maybeValueIt != portValues.end()) {
            return maybeValueIt->second;
        }

        return staticNaN;
    }

    void BaseBlock::setPortValue(const gui::elements::base::Port* port,
                                 const boost::multiprecision::cpp_dec_float_50& value) {
        portValues[port] = value;
    }
}  // namespace gui::elements::base
