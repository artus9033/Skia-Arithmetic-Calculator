#ifndef GUI_GEOMETRY_POINT2D_H
#define GUI_GEOMETRY_POINT2D_H

namespace gui::geometry {
    /**
     * @brief A basic struct representing a 2D point
     */
    struct Point2D {
       public:
        /**
         * The x coordinate
         */
        int x;

        /**
         * The y coordinate
         */
        int y;
    };
}  // namespace gui::geometry

#endif  // GUI_GEOMETRY_POINT2D_H
