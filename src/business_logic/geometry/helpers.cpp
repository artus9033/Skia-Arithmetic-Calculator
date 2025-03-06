#include "helpers.h"

namespace business_logic::geometry {
    bool isCircleHovered(int mouseX, int mouseY, int cX, int cY, int radius) {
        // for simplicity, performance and usability, check if the mouse is within a square
        // that bounds the circle
        return mouseX >= cX - radius && mouseX <= cX + radius && mouseY >= cY - radius &&
               mouseY <= cY + radius;
    }
}  // namespace business_logic::geometry
