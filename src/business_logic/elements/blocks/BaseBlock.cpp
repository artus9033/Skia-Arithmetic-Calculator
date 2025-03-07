#include "BaseBlock.h"

/**
 * @brief The base classes and structs for elements
 */
namespace business_logic::elements::blocks {

    BaseBlock::BaseBlock(
        int cx,
        int cy,
        int blockWidth,
        int blockHeight,
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        business_logic::delegate::IWindowDelegate* windowDelegate,
        std::shared_ptr<spdlog::logger> logger)
        : width(blockWidth),
          height(blockHeight),
          logger(logger),
          newBlockChoiceDelegate(newBlockChoiceDelegate),
          blockLifecycleManagerDelegate(blockLifecycleManagerDelegate),
          windowDelegate(windowDelegate) {
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
        auto windowSize = windowDelegate->getWindowSize();

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

    std::optional<std::string> BaseBlock::getValueToRenderAboveBlock(
        [[maybe_unused]] bool isHovered) {
        auto& outputPorts = getOutputPorts();

        // if more or less than 1 output port is present, do not render the value above the block
        if (outputPorts.size() != 1) {
            return std::nullopt;
        }

        auto blockValue = getPortValue(&(outputPorts[0]));

        std::ostringstream oss;
        oss << std::scientific
            << std::setprecision(business_logic::constants::DEFAULT_VALUE_DISPLAY_PRECISION)
            << blockValue;

        return oss.str();
    }

    const business_logic::geometry::Point2D& BaseBlock::getPortCoordinates(
        const business_logic::elements::structures::Port* port) const {
        // trick to be able to return a static reference for this function; also optimizes the
        // function call
        static const geometry::Point2D zeroPoint = {.x = 0, .y = 0};

        if (portCoordinates.find(port) != portCoordinates.end()) {
            return portCoordinates.at(port);
        }

        return zeroPoint;
    }

    std::optional<const business_logic::elements::structures::Port*> BaseBlock::checkPort(
        const business_logic::elements::structures::Port* port,
        const geometry::Point2D& point) const {
        auto& predCoordinates = getPortCoordinates(port);

        if (business_logic::geometry::isCircleHovered(point.x,
                                                      point.y,
                                                      predCoordinates.x,
                                                      predCoordinates.y,
                                                      constants::TOTAL_PORT_HITBOX_RADIUS)) {
            return std::make_optional(port);
        }

        return std::nullopt;
    }

    std::optional<const business_logic::elements::structures::Port*>
    BaseBlock::getPortAtCoordinates(const geometry::Point2D& point) const {
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

    const FloatingPoint& BaseBlock::getPortValue(
        const business_logic::elements::structures::Port* port) const {
        auto maybeValueIt = portValues.find(port);

        if (maybeValueIt != portValues.end()) {
            return maybeValueIt->second;
        }

        return NaN;
    }

    void BaseBlock::setPortValue(const business_logic::elements::structures::Port* port,
                                 const FloatingPoint& value) {
        portValues[port] = value;
    }

    const FloatingPoint BaseBlock::NaN = std::numeric_limits<FloatingPoint>::quiet_NaN();
}  // namespace business_logic::elements::blocks
