#ifndef GUI_LOGIC_DELEGATE_IBLOCK_LIFECYCLE_MANAGER_DELEGATE_H
#define GUI_LOGIC_DELEGATE_IBLOCK_LIFECYCLE_MANAGER_DELEGATE_H

#include <optional>

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/logic/PortsConnectionSide.h"

namespace gui::elements::base {
    class BaseBlock;
}

namespace gui::logic::delegate {
    /**
     * @brief Delegate that manages the lifecycles of blocks
     */
    class IBlockLifecycleManagerDelegate {
       public:
        virtual ~IBlockLifecycleManagerDelegate() = default;

        /**
         * @brief Invoked when a connection is made between two ports + blocks (sides)
         * @param source The source side of the connection
         * @param dest The destination side of the connection
         */
        virtual void onPortsConnected(const gui::logic::PortsConnectionSide& source,
                                      const gui::logic::PortsConnectionSide& dest) = 0;

        /**
         * @brief Checks if there is a connection between two entities
         * @param source The source side of the connection
         * @param dest The destination side of the connection
         * @return True if there is a connection between the two entities, false otherwise
         */
        virtual bool hasConnectionBetween(const gui::logic::PortsConnectionSide& source,
                                          const gui::logic::PortsConnectionSide& dest) const = 0;

        /**
         * @brief Checks if the input port is connected to anything
         * @param side The side to check
         * @return True if the input port is connected to anything, false otherwise
         */
        virtual bool isInputConnected(const gui::logic::PortsConnectionSide& side) const = 0;

        /**
         * @brief Called when a block is deleted
         * @param block The block that was deleted
         */
        virtual void onBlockDeleted(const gui::elements::base::BaseBlock* block) = 0;
    };
}  // namespace gui::logic::delegate

#endif  // GUI_LOGIC_DELEGATE_IBLOCK_LIFECYCLE_MANAGER_DELEGATE_H
