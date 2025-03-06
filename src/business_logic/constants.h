#ifndef BUSINESS_LOGIC_CONSTANTS_H
#define BUSINESS_LOGIC_CONSTANTS_H

namespace business_logic::constants {
    // Port-related constants
    constexpr int PORT_CIRCLE_RADIUS = 10;
    static_assert(PORT_CIRCLE_RADIUS > 0, "PORT_CIRCLE_RADIUS must be greater than 0");
    static_assert(PORT_CIRCLE_RADIUS % 2 == 0, "PORT_CIRCLE_RADIUS must be divisible by 2");

    constexpr int PORT_CIRCLE_RADIUS_HALF = PORT_CIRCLE_RADIUS / 2;

    constexpr int PORT_CIRCLE_OUTLINE_WIDTH = 6;
    static_assert(PORT_CIRCLE_OUTLINE_WIDTH > 0,
                  "PORT_CIRCLE_OUTLINE_WIDTH must be greater than 0");
    static_assert(PORT_CIRCLE_OUTLINE_WIDTH < PORT_CIRCLE_RADIUS,
                  "PORT_CIRCLE_OUTLINE_WIDTH must be less than PORT_CIRCLE_RADIUS");

    constexpr int TOTAL_PORT_HITBOX_RADIUS = PORT_CIRCLE_RADIUS + PORT_CIRCLE_OUTLINE_WIDTH / 2;
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
    constexpr double DOUBLE_CLICK_TIME_THRESHOLD_SECONDS = 0.2;

}  // namespace business_logic::constants

#endif  // BUSINESS_LOGIC_CONSTANTS_H
