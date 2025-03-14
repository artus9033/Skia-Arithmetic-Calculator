#ifndef BUSINESS_LOGIC_ELEMENTS_BLOCKS_BASE_BLOCK_H
#define BUSINESS_LOGIC_ELEMENTS_BLOCKS_BASE_BLOCK_H

#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "delegate/IBlockLifecycleManagerDelegate.h"
#include "delegate/INewBlockChoiceDelegate.h"
#include "delegate/IWindowDelegate.h"
#include "elements/blocks/BlockType.h"
#include "elements/interactions/IDraggable.h"
#include "geometry/Point2D.h"
#include "geometry/helpers.h"
#include "logging/Loggable.h"
#include "structures/Port.h"
#include "typenames.h"

namespace business_logic::delegate {
    class IBlockLifecycleManagerDelegate;  // forward declaration to satisfy clang-tidy
}

/**
 * @brief The elements (i.e., renderable entities constituting for the calculator) module
 */
namespace business_logic::elements {}

namespace business_logic::elements::blocks {
    /**
     * @brief The base class for all blocks, containing common functionality and members
     *
     * Any block subclassing this class can be dragged around to change its position
     */
    class BaseBlock : public business_logic::elements::interactions::IDraggable {
       public:
        /**
         * @brief Constructor
         * @param cx The center x coordinate where to place the center of the block
         * @param cy The center y coordinate where to place the center of the block
         * @param blockWidth The width of the block
         * @param blockHeight The height of the block
         * @param newBlockChoiceDelegate The delegate that is notified when a new block is chosen to
         * be added to the canvas
         * @param blockLifecycleManagerDelegate The delegate that manages the lifecycles of blocks
         * @param windowDelegate The delegate of the window
         * @param logger The logger for the block
         */
        BaseBlock(
            int cx,
            int cy,
            int blockWidth,
            int blockHeight,
            business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            // NOLINTBEGIN
            business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            // NOLINTEND
            business_logic::delegate::IWindowDelegate* windowDelegate,
            std::shared_ptr<spdlog::logger> logger);

        /**
         * @brief Updates the width and height of the block
         * @param newWidth The new width of the block
         * @param newHeight The new height of the block
         */
        void updateWidthHeight(int newWidth, int newHeight);

        ~BaseBlock() noexcept override;

        // disable copy semantics
        BaseBlock(const BaseBlock&) = delete;
        BaseBlock& operator=(const BaseBlock&) = delete;

        // disable move semantics
        BaseBlock(BaseBlock&&) = delete;
        BaseBlock& operator=(BaseBlock&&) = delete;

        /**
         * @brief Checks if the block is hovered over by the mouse
         * @param x The x coordinate of the mouse
         * @param y The y coordinate of the mouse
         * @return True if the block is hovered over, false otherwise
         */
        virtual bool isHovered(int x, int y) const {
            return x >= leftX && x <= rightX && y >= topY && y <= bottomY;
        }

        /**
         * @brief Wrapper that gets the value of the output port above the block; if more or less
         * than 1 output port is present, it will not be rendered
         * @param isHovered True if the block is hovered over, false otherwise
         * @return The value to render above the block or `std::nullopt` if the value should not be
         * rendered
         */
        virtual std::optional<std::string> getValueToRenderAboveBlock(
            [[maybe_unused]] bool isHovered);

        /**
         * \copydoc business_logic::elements::interactions::IDraggable::onDragStart
         */
        void onDragStart() override;

        /**
         * \copydoc business_logic::elements::interactions::IDraggable::onDragProgress
         */
        void onDragProgress(int x, int y) override;

        /**
         * \copydoc business_logic::elements::interactions::IDraggable::onDragEnd
         */
        void onDragEnd() override;

        /**
         * @brief Gets the coordinates of a port
         * @param port The port to get the coordinates of
         * @return The coordinates of the port
         */
        const business_logic::geometry::Point2D& getPortCoordinates(
            const structures::Port* port) const;

        /**
         * @brief Gets the port at given coordinates
         * @param point The coordinates to check
         * @return An optional carrying the Port if it is hit or `std::nullopt` otherwise
         */
        std::optional<const structures::Port*> getPortAtCoordinates(
            const geometry::Point2D& point) const;

        /**
         * The unique identifier of the block (its address in memory), used for logging
         * purposes; should return the same value as `business_logic::stringifyAddressOf(this)`
         */
        virtual std::string getSelfId() const = 0;

        /**
         * @brief Gets the type of the block
         * @return The type of the block
         */
        virtual BlockType getBlockType() const = 0;

        /**
         * @brief Gets the value of the port
         * @param port The port to get the value of
         * @return The value of the port or NaN if the port is not connected / not in the registry
         */
        const FloatingPoint& getPortValue(const structures::Port* port) const;

        /**
         * @brief Sets the value of the port
         * @param port The port to set the value of
         * @param value The value to set the port to
         */
        void setPortValue(const business_logic::elements::structures::Port* port,
                          const FloatingPoint& value);

        /**
         * @brief Calculates the output port values based on input port values
         * @throws std::runtime_error if input values are missing
         */
        virtual void calculateOutputValues() = 0;

        // make the hash function a friend to allow it to access non-public members
        friend struct std::hash<BaseBlock>;

        /**
         * @brief Gets the input ports of the block
         * @return Vector of input ports
         */
        virtual const std::vector<structures::Port>& getInputPorts() const = 0;

        /**
         * @brief Gets the output ports of the block
         * @return Vector of output ports
         */
        virtual const std::vector<structures::Port>& getOutputPorts() const = 0;

        /**
         * @brief Gets the center y coordinate of the block
         * @return The center y coordinate of the block
         */
        int getCx() const { return cx; }

        /**
         * @brief Gets the center y coordinate of the block
         * @return The center y coordinate of the block
         */
        int getCy() const { return cy; }

        /**
         * @brief Gets the left x coordinate of the block
         * @return The left x coordinate of the block
         */
        int getLeftX() const { return leftX; }

        /**
         * @brief Gets the right x coordinate of the block
         * @return The right x coordinate of the block
         */
        int getRightX() const { return rightX; }

        /**
         * @brief Gets the top y coordinate of the block
         * @return The top y coordinate of the block
         */
        int getTopY() const { return topY; }

        /**
         * @brief Gets the bottom y coordinate of the block
         * @return The bottom y coordinate of the block
         */
        int getBottomY() const { return bottomY; }

        /**
         * @brief Gets the width of the block
         * @return The width of the block
         */
        int getWidth() const { return width; }

        /**
         * @brief Gets the height of the block
         * @return The height of the block
         */
        int getHeight() const {  // cppcheck-suppress unusedFunction
            return height;
        }

        /**
         * @brief Gets the cache of (center) coordinates of the ports of the block; updated after
         * rendering
         */
        std::unordered_map<const structures::Port*, geometry::Point2D>& getPortCoordinates() {
            return portCoordinates;
        }

       protected:
        /**
         * The center x coordinate of the block
         */
        int cx;

        /**
         * The center y coordinate of the block
         */
        int cy;

        /**
         * The width of the block
         */
        int width;

        /**
         * The height of the block
         */
        int height;

        /**
         * The left edge x coordinate of the block; cached and used elsewhere
         */
        int leftX;

        /**
         * The right edge x of the block; cached and used for the needs of `isHovered()`
         */
        int rightX;

        /**
         * The top edge y coordinate of the block; cached and used elsewhere
         */
        int topY;

        /**
         * The bottom edge y of the block; cached and used for the needs of `isHovered()`
         */
        int bottomY;

        /**
         * The center y coordinate of the block; cached and used elsewhere
         */
        float centerY;

        /**
         * The logger for the block
         */
        std::shared_ptr<spdlog::logger> logger;

        /**
         * @brief The registry of port values
         */
        std::unordered_map<const structures::Port*, FloatingPoint> portValues;

        /**
         * The delegate that is notified when a new block is chosen to be added to the canvas
         */
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate;

        // NOLINTBEGIN
        /**
         * The delegate that manages the lifecycles of blocks
         */
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate;
        // NOLINTEND

        /**
         * The delegate of the window
         */
        business_logic::delegate::IWindowDelegate* windowDelegate;

        /**
         * The NaN value - for convenience of usage in blocks implementations
         */
        static const FloatingPoint NaN;

        /**
         * @brief Caches the (center) coordinates of the ports of the block; must be updated after
         * rendering
         */
        std::unordered_map<const structures::Port*, geometry::Point2D> portCoordinates;

        /**
         * @brief Caches the bottom right corner of the block, so that it is not recalculated every
         * time `isHovered()` is called
         */
        void cacheCornerCoordinates();

        /**
         * @brief Checks if a point is inside the hitbox of a port
         * @param port The port that hitbox of is to be checked
         * @param point The point to check whether it is inside the hitbox of the port
         * @return An optional carrying the Port if it is hit or `std::nullopt` otherwise
         */
        std::optional<const structures::Port*> checkPort(const structures::Port* port,
                                                         const geometry::Point2D& point) const;
    };
}  // namespace business_logic::elements::blocks

namespace std {
    /**
     * @brief The hash function is required for std unordered containers
     */
    template <>
    struct hash<business_logic::elements::blocks::BaseBlock> {
        std::size_t operator()(const business_logic::elements::blocks::BaseBlock& p) const {
            return std::hash<int>()(p.cx) ^ (std::hash<int>()(p.cy) << 1U);
        }
    };
}  // namespace std

#endif  // BUSINESS_LOGIC_ELEMENTS_BLOCKS_BASE_BLOCK_H
