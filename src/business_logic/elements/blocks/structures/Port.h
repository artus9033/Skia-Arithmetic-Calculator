#ifndef BUSINESS_LOGIC_ELEMENTS_BLOCKS_STRUCTURES_PORT_H
#define BUSINESS_LOGIC_ELEMENTS_BLOCKS_STRUCTURES_PORT_H

#include <cstdint>
#include <string>

namespace business_logic::elements::structures {
    /**
     * @brief Represents a port on a block
     */
    struct Port {
       public:
        /**
         * @brief The name/label of the port
         */
        std::string name;

        /**
         * @brief The type of the port
         */
        enum class Type : std::uint8_t { INPUT, OUTPUT };

        /**
         * @brief The type of the port
         */
        Type type;
    };
}  // namespace business_logic::elements::structures

#endif  // BUSINESS_LOGIC_ELEMENTS_BLOCKS_STRUCTURES_PORT_H
