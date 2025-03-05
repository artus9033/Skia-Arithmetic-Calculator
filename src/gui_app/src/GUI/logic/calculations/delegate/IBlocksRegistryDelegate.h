#ifndef IBLOCKS_REGISTRY_DELEGATE_H
#define IBLOCKS_REGISTRY_DELEGATE_H

#include <unordered_map>
#include <unordered_set>

#include "GUI/logic/PortsConnectionSide.h"

namespace gui::logic::calculations::delegate {
    /**
     * @brief The delegate for the blocks registry, allowing to get connections and blocks
     */

    class IBlocksRegistryDelegate {
       public:
        /**
         * @brief Gets the blocks
         * @return The blocks
         */
        virtual const std::vector<std::shared_ptr<gui::elements::base::BaseBlock>>& getBlocks()
            const = 0;

        /**
         * @brief Gets the connections registry
         * @return The connections registry
         */
        virtual const std::unordered_map<gui::logic::PortsConnectionSide,
                                         std::unordered_set<gui::logic::PortsConnectionSide>>&
        getConnectionsRegistry() const = 0;
    };

}  // namespace gui::logic::calculations::delegate
#endif  // IBLOCKS_REGISTRY_DELEGATE_H
