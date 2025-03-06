#ifndef IBLOCKS_REGISTRY_DELEGATE_H
#define IBLOCKS_REGISTRY_DELEGATE_H

#include <unordered_map>
#include <unordered_set>

#include "elements/blocks/structures/BlocksConnectionSide.h"

namespace business_logic::calculations::delegate {
    /**
     * @brief The delegate for the blocks registry, allowing to get connections and blocks
     */

    class IBlocksRegistryDelegate {
       public:
        /**
         * @brief Gets the blocks
         * @return The blocks
         */
        virtual const std::vector<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>&
        getBlocks() const = 0;

        /**
         * @brief Gets the connections registry
         * @return The connections registry
         */
        virtual const std::unordered_map<
            business_logic::elements::structures::BlocksConnectionSide,
            std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>>&
        getConnectionsRegistry() const = 0;
    };

}  // namespace business_logic::calculations::delegate
#endif  // IBLOCKS_REGISTRY_DELEGATE_H
