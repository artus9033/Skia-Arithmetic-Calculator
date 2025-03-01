#ifndef GUI_GEOMETRY_HELPERS_H
#define GUI_GEOMETRY_HELPERS_H

#include <skia/include/core/SkCanvas.h>

namespace gui::geometry {
    /**
     * @brief Check if a point is within a circle
     * @param mouseX The x coordinate of the mouse
     * @param mouseY The y coordinate of the mouse
     * @param cX The x coordinate of the center of the circle
     * @param cY The y coordinate of the center of the circle
     * @param radius The radius of the circle
     * @return True if the point is within the circle, false otherwise
     */
    bool isCircleHovered(int mouseX, int mouseY, int cX, int cY, int radius);
}  // namespace gui::geometry

#endif  // GUI_GEOMETRY_HELPERS_H
