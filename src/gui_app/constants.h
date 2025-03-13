#ifndef GUI_CONSTANTS_H
#define GUI_CONSTANTS_H

/**
 * @brief Namespace defining GUI constants
 */
namespace gui::constants {
    constexpr int MENU_HEADLINE_FONT_SIZE_BASE = 17;
    constexpr int MENU_CAPTION_FONT_SIZE_BASE = 10;
    constexpr int CAPTION_FONT_SIZE_BASE = 7;
    constexpr int MENU_CHOICE_FONT_SIZE_BASE = 14;

    /**
     * @brief The base window width that the current width is divided by to obtain a scaling factor
     * for font size
     */
    constexpr int FONT_ASPECT_BASE_WINDOW_WIDTH = 800;
    /**
     * @brief The base window height that the current height is divided by to obtain a scaling
     * factor for font size
     */
    constexpr int FONT_ASPECT_BASE_WINDOW_HEIGHT = 600;
}  // namespace gui::constants

#endif  // GUI_CONSTANTS_H
