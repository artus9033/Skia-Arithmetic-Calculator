#ifndef GUI_ELEMENTS_BASE_I_BLOCK_H
#define GUI_ELEMENTS_BASE_I_BLOCK_H

#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMgr.h>

#include "skia/include/core/SkTypeface.h"

#ifdef __APPLE__
#include "skia/include/ports/SkFontMgr_mac_ct.h"
#endif

#include <memory>
#include <unordered_map>
#include <vector>

#include "GUI/geometry/Point2D.h"
#include "GUI/geometry/Size2D.h"
#include "GUI/geometry/helpers.h"
#include "GUI/renderer/colors.h"
#include "IDraggable.h"
#include "logging/Loggable.h"

#define CAPTION_FONT_SIZE 8  // TODO: replace this with a centralized registry of static fonts

// note: the assumption is that PORT_CIRCLE_RADIUS is divisible by 2 (int arithmetic for performance
// reasons)
#define PORT_CIRCLE_RADIUS 10
static_assert(PORT_CIRCLE_RADIUS > 0, "PORT_CIRCLE_RADIUS must be greater than 0");
static_assert(PORT_CIRCLE_RADIUS % 2 == 0, "PORT_CIRCLE_RADIUS must be divisible by 2");

#define PORT_CIRCLE_RADIUS_HALF PORT_CIRCLE_RADIUS / 2

#define PORT_CIRCLE_OUTLINE_WIDTH 6
static_assert(PORT_CIRCLE_OUTLINE_WIDTH > 0, "PORT_CIRCLE_OUTLINE_WIDTH must be greater than 0");
static_assert(PORT_CIRCLE_OUTLINE_WIDTH < PORT_CIRCLE_RADIUS,
              "PORT_CIRCLE_OUTLINE_WIDTH must be less than PORT_CIRCLE_RADIUS");

#define TOTAL_PORT_HITBOX_RADIUS (PORT_CIRCLE_RADIUS + PORT_CIRCLE_OUTLINE_WIDTH / 2)
#define TOTAL_PORT_RADIUS (PORT_CIRCLE_RADIUS + PORT_CIRCLE_OUTLINE_WIDTH)
#define TOTAL_PORT_RADIUS_HALF TOTAL_PORT_RADIUS / 2

#define PORT_CIRCLE_MARGIN PORT_CIRCLE_RADIUS / 2
#define PORT_CIRCLE_MARGIN_HALF PORT_CIRCLE_MARGIN / 2

// note: the assumption is that BLOCK_OUTLINE_WIDTH is divisible by 2 (int arithmetic for
// performance reasons)
#define BLOCK_OUTLINE_WIDTH 4
static_assert(BLOCK_OUTLINE_WIDTH > 0, "BLOCK_OUTLINE_WIDTH must be greater than 0");
static_assert(BLOCK_OUTLINE_WIDTH % 2 == 0, "BLOCK_OUTLINE_WIDTH must be divisible by 2");

#define HALF_BLOCK_OUTLINE_WIDTH BLOCK_OUTLINE_WIDTH / 2

namespace gui::elements::base {
    // using an empty namespace to avoid polluting the outer scope of gui::elements::base
    namespace {
        namespace colors = gui::renderer::colors;
    }

    /**
     * @brief Represents a port on a block
     */
    struct Port {
       public:
        /**
         * @brief The name/label of the port
         */
        std::string name;

        /**
         * @brief The type of the port
         */
        enum class Type { INPUT, OUTPUT };

        /**
         * @brief The type of the port
         */
        Type type;
    };

    class BaseBlock : public IDraggable {
       public:
        /**
         * @brief Constructor
         * @param cx The center x coordinate where to place the center of the block
         * @param cy The center y coordinate where to place the center of the block
         * @param blockWidth The width of the block
         * @param blockHeight The height of the block
         * @param windowSize The size of the window
         */
        BaseBlock(int cx,
                  int cy,
                  int blockWidth,
                  int blockHeight,
                  std::shared_ptr<spdlog::logger> logger,
                  const geometry::Size2D& windowSize);

        /**
         * @brief Updates the width and height of the block
         * @param newWidth The new width of the block
         * @param newHeight The new height of the block
         */
        void updateWidthHeight(int newWidth, int newHeight);

        virtual ~BaseBlock() = default;

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
         * @brief Renders the block
         * @param canvas The canvas to render the block on
         * @param mouseX The x coordinate of the mouse
         * @param mouseY The y coordinate of the mouse
         * @param isHovered True if the block is hovered over, false otherwise
         */
        virtual void render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered);

        /**
         * \copydoc IDraggable::onDragStart
         */
        void onDragStart(int x, int y) override;

        /**
         * \copydoc IDraggable::onDragProgress
         */
        void onDragProgress(int x, int y) override;

        /**
         * \copydoc IDraggable::onDragEnd
         */
        void onDragEnd(int x, int y) override;

        /**
         * @brief Gets the coordinates of a port
         * @param port The port to get the coordinates of
         * @return The coordinates of the port
         */
        geometry::Point2D getPortCoordinates(const gui::elements::base::Port* port) const;

        /**
         * @brief Gets the port at given coordinates
         * @param point The coordinates to check
         * @return An optional carrying the Port if it is hit or `std::nullopt` otherwise
         */
        std::optional<const gui::elements::base::Port*> getPortAtCoordinates(
            const geometry::Point2D& point) const;

        /**
         * The unique identifier of the block (its address in memory), used for logging
         * purposes; should return the same value as `business_logic::stringifyAddressOf(this)`
         */
        virtual std::string getSelfId() const = 0;

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
         * The size of the window
         */
        geometry::Size2D windowSize;

        /**
         * @brief Gets the input ports of the block
         * @return Vector of input ports
         */
        virtual const std::vector<Port>& getInputPorts() const = 0;

        /**
         * @brief Gets the output ports of the block
         * @return Vector of output ports
         */
        virtual const std::vector<Port>& getOutputPorts() const = 0;

        /**
         * The logger for the block
         */
        std::shared_ptr<spdlog::logger> logger;

       private:
        /**
         * @brief Caches the coordinates of the ports of the block; must be updated after rendering
         */
        std::unordered_map<const gui::elements::base::Port*, geometry::Point2D> portCoordinates;

        /**
         * @brief Caches the bottom right corner of the block, so that it is not recalculated every
         * time `isHovered()` is called
         */
        void cacheCornerCoordinates();

        /**
         * @brief Checks if a point is inside the hitbox of a port
         * @param portsCollection The collection of ports to check
         * @param point The point to check
         * @return An optional carrying the Port if it is hit or `std::nullopt` otherwise
         */
        std::optional<const gui::elements::base::Port*> checkPort(
            const gui::elements::base::Port* port, const geometry::Point2D& point) const;
    };
}  // namespace gui::elements::base

#endif  // GUI_ELEMENTS_BASE_I_BLOCK_H
