#ifndef BUSINESS_LOGIC_GEOMETRY_SIZE2D_H
#define BUSINESS_LOGIC_GEOMETRY_SIZE2D_H

namespace business_logic::geometry {
    /**
     * @brief A basic struct representing a 2D size
     */
    struct Size2D final {
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
}  // namespace business_logic::geometry

#endif  // BUSINESS_LOGIC_GEOMETRY_SIZE2D_H
