#ifndef GUI_GEOMETRY_SIZE2D_H
#define GUI_GEOMETRY_SIZE2D_H

namespace gui::geometry {
    /**
     * @brief A basic struct representing a 2D size
     */
    struct Size2D {
       public:
        /**
         * The width
         */
        int width;

        /**
         * The height
         */
        int height;
    };
}  // namespace gui::geometry

#endif  // GUI_GEOMETRY_SIZE2D_H
