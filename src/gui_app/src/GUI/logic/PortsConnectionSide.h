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
    struct PortsConnectionSide {
        gui::elements::base::BaseBlock* block;
        const gui::elements::base::Port* port;

        // the equality operator is required for std unordered containers
        bool operator==(const PortsConnectionSide& other) const {
            return block == other.block && port == other.port;
        }
    };

    using PortsConnection = std::pair<PortsConnectionSide, PortsConnectionSide>;
}  // namespace gui::logic

// the hash function is required for std unordered containers
namespace std {
    template <>
    struct hash<gui::logic::PortsConnectionSide> {
        std::size_t operator()(const gui::logic::PortsConnectionSide& p) const {
            return std::hash<gui::elements::base::BaseBlock*>()(p.block) ^
                   (std::hash<const gui::elements::base::Port*>()(p.port) << 1);
        }
    };
}  // namespace std

#endif  // GUI_LOGIC_PORTS_CONNECTION_SIDE_H
