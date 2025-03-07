#ifndef BUSINESS_LOGIC_ELEMENTS_BLOCKS_STRUCTURES_BLOCKS_CONNECTION_SIDE_H
#define BUSINESS_LOGIC_ELEMENTS_BLOCKS_STRUCTURES_BLOCKS_CONNECTION_SIDE_H

#include "elements/blocks/structures/Port.h"

// forward declaration
namespace business_logic::elements::blocks {
    class BaseBlock;
}

/**
 * @brief Abstract structures that constitute for the calculator GUI and are logical models of the
 * rendered entities
 */
namespace business_logic::elements::structures {
    // forward declaration
    struct Port;

    /**
     * @brief A side of a connection between two ports
     */
    struct BlocksConnectionSide {
        /**
         * @brief The block
         */
        business_logic::elements::blocks::BaseBlock* block;

        /**
         * @brief The port
         */
        const business_logic::elements::structures::Port* port;

        /**
         * @brief Equality operator
         *
         * The equality operator is required for std unordered containers.
         * @param other The other side
         * @return True if the sides are equal, false otherwise
         */
        bool operator==(const BlocksConnectionSide& other) const {
            return block == other.block && port == other.port;
        }
    };

    using PortsConnection = std::pair<BlocksConnectionSide, BlocksConnectionSide>;
}  // namespace business_logic::elements::structures

namespace std {
    /**
     * @brief The hash function is required for std unordered containers
     */
    template <>
    struct hash<business_logic::elements::structures::BlocksConnectionSide> {
        std::size_t operator()(
            const business_logic::elements::structures::BlocksConnectionSide& p) const {
            return std::hash<business_logic::elements::blocks::BaseBlock*>()(p.block) ^
                   (std::hash<const business_logic::elements::structures::Port*>()(p.port) << 1);
        }
    };
}  // namespace std

#endif  // BUSINESS_LOGIC_ELEMENTS_BLOCKS_STRUCTURES_BLOCKS_CONNECTION_SIDE_H
