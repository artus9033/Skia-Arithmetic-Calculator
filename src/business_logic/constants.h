#ifndef BUSINESS_LOGIC_CONSTANTS_H
#define BUSINESS_LOGIC_CONSTANTS_H

#include <chrono>

/**
 * @brief General logic constants
 */
namespace business_logic::constants {
    // Port-related constants; note: most of these constants could have been moved to the GUI
    // module, however most of them are dependencies of TOTAL_PORT_HITBOX_RADIUS, which is needed in
    // the business_logic module; therefore, for coherence, all of them were decided upon to stay in
    // this file
    constexpr int PORT_CIRCLE_RADIUS = 10;
    static_assert(PORT_CIRCLE_RADIUS > 0, "PORT_CIRCLE_RADIUS must be greater than 0");
    static_assert(PORT_CIRCLE_RADIUS % 2 == 0, "PORT_CIRCLE_RADIUS must be divisible by 2");

    constexpr int PORT_CIRCLE_RADIUS_HALF = PORT_CIRCLE_RADIUS / 2;

    constexpr int PORT_CIRCLE_OUTLINE_WIDTH = 6;
    static_assert(PORT_CIRCLE_OUTLINE_WIDTH > 0,
                  "PORT_CIRCLE_OUTLINE_WIDTH must be greater than 0");
    static_assert(PORT_CIRCLE_OUTLINE_WIDTH < PORT_CIRCLE_RADIUS,
                  "PORT_CIRCLE_OUTLINE_WIDTH must be less than PORT_CIRCLE_RADIUS");

    constexpr int TOTAL_PORT_HITBOX_RADIUS = PORT_CIRCLE_RADIUS + (PORT_CIRCLE_OUTLINE_WIDTH / 2);
    constexpr int TOTAL_PORT_RADIUS = PORT_CIRCLE_RADIUS + PORT_CIRCLE_OUTLINE_WIDTH;
    constexpr int TOTAL_PORT_RADIUS_HALF = TOTAL_PORT_RADIUS / 2;

    constexpr int PORT_CIRCLE_MARGIN = 0;  // PORT_CIRCLE_RADIUS / 2
    constexpr int PORT_CIRCLE_MARGIN_HALF = PORT_CIRCLE_MARGIN / 2;

    // Block-related constants
    constexpr int BLOCK_OUTLINE_WIDTH = 4;
    static_assert(BLOCK_OUTLINE_WIDTH > 0, "BLOCK_OUTLINE_WIDTH must be greater than 0");
    static_assert(BLOCK_OUTLINE_WIDTH % 2 == 0, "BLOCK_OUTLINE_WIDTH must be divisible by 2");

    constexpr int MAX_INPUT_CHOICES_PER_ROW = 3;
    constexpr int DEFAULT_VALUE_DISPLAY_PRECISION = 10;
    /**
     * The upper boundary threshold up to which (inclusive) a second click interaction must follow
     * a preceding one for the whole action to be counted as a double click
     */
    constexpr std::chrono::milliseconds DOUBLE_CLICK_TIME_THRESHOLD_MS(200);

}  // namespace business_logic::constants

#endif  // BUSINESS_LOGIC_CONSTANTS_H
