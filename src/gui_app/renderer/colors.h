#ifndef GUI_RENDERER_COLORS_H
#define GUI_RENDERER_COLORS_H

#include <skia/include/core/SkColor.h>

/**
 * @brief Color constants used for rendering entities
 */
namespace gui::renderer::colors {

    // purple-blue
    extern const SkColor PURPLE_BLUE;

    // red
    extern const SkColor RED;

    // gray
    extern const SkColor WINDOW_BACKGROUND_COLOR;

    // dark blue
    extern const SkColor BLOCK_OUTLINE_COLOR;

    // red
    extern const SkColor BLOCK_HOVERED_OUTLINE_COLOR;

    // light blue gray
    extern const SkColor BLOCK_BACKGROUND_COLOR;

    // dark gray
    extern const SkColor BLOCK_PORT_OUTLINE_COLOR;

    // white
    extern const SkColor TEXT_COLOR;

    // black
    extern const SkColor TEXT_OUTLINE_COLOR;

}  // namespace gui::renderer::colors

#endif  // GUI_RENDERER_COLORS_H
