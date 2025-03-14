#ifndef BUSINESS_LOGIC_LOGIC_DELEGATE_IBLOCK_LIFECYCLE_MANAGER_DELEGATE_H
#define BUSINESS_LOGIC_LOGIC_DELEGATE_IBLOCK_LIFECYCLE_MANAGER_DELEGATE_H

#include <optional>

#include "elements/blocks/BaseBlock.h"
#include "elements/blocks/structures/BlocksConnectionSide.h"

// forward declaration
namespace business_logic::elements::blocks {
    class BaseBlock;
}

namespace business_logic::delegate {
    /**
     * @brief Delegate that manages the lifecycles of blocks
     */
    class
        IBlockLifecycleManagerDelegate {  // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
       public:
        virtual ~IBlockLifecycleManagerDelegate() = default;

        /**
         * @brief Invoked when a connection is made between two ports + blocks (sides)
         * @param source The source side of the connection
         * @param dest The destination side of the connection
         */
        virtual void onPortsConnected(
            const business_logic::elements::structures::BlocksConnectionSide& source,
            const business_logic::elements::structures::BlocksConnectionSide& dest) = 0;

        /**
         * @brief Checks if there is a connection between two entities
         * @param source The source side of the connection
         * @param dest The destination side of the connection
         * @return True if there is a connection between the two entities, false otherwise
         */
        virtual bool hasConnectionBetween(
            const business_logic::elements::structures::BlocksConnectionSide& source,
            const business_logic::elements::structures::BlocksConnectionSide& dest) const = 0;

        /**
         * @brief Checks if the input port is connected to anything
         * @param side The side to check
         * @return True if the input port is connected to anything, false otherwise
         */
        virtual bool isInputConnected(
            const business_logic::elements::structures::BlocksConnectionSide& side) const = 0;

        /**
         * @brief Called when a block is deleted
         * @param block The block that was deleted
         */
        virtual void onBlockDeleted(const business_logic::elements::blocks::BaseBlock* block) = 0;
    };
}  // namespace business_logic::delegate

#endif  // BUSINESS_LOGIC_LOGIC_DELEGATE_IBLOCK_LIFECYCLE_MANAGER_DELEGATE_H
