#ifndef GUI_LOGIC_PORTS_CONNECTION_SIDE_H
#define GUI_LOGIC_PORTS_CONNECTION_SIDE_H

#include <utility>

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/elements/base/Port.h"

// forward declaration
namespace gui::elements::base {
    class BaseBlock;
    struct Port;
}  // namespace gui::elements::base

namespace gui::logic {
    /**
     * @brief A side of a connection between two ports
     */
    struct PortsConnectionSide {
        /**
         * @brief The block
         */
        gui::elements::base::BaseBlock* block;

        /**
         * @brief The port
         */
        const gui::elements::base::Port* port;

        /**
         * @brief Equality operator
         *
         * The equality operator is required for std unordered containers.
         * @param other The other side
         * @return True if the sides are equal, false otherwise
         */
        bool operator==(const PortsConnectionSide& other) const {
            return block == other.block && port == other.port;
        }
    };

    using PortsConnection = std::pair<PortsConnectionSide, PortsConnectionSide>;
}  // namespace gui::logic

namespace std {
    /**
     * @brief The hash function is required for std unordered containers
     */
    template <>
    struct hash<gui::logic::PortsConnectionSide> {
        std::size_t operator()(const gui::logic::PortsConnectionSide& p) const {
            return std::hash<gui::elements::base::BaseBlock*>()(p.block) ^
                   (std::hash<const gui::elements::base::Port*>()(p.port) << 1);
        }
    };
}  // namespace std

#endif  // GUI_LOGIC_PORTS_CONNECTION_SIDE_H
