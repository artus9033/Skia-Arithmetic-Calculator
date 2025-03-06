#ifndef BUSINESS_LOGIC_GEOMETRY_POINT2D_H
#define BUSINESS_LOGIC_GEOMETRY_POINT2D_H

namespace business_logic::geometry {
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
}  // namespace business_logic::geometry

#endif  // BUSINESS_LOGIC_GEOMETRY_POINT2D_H
