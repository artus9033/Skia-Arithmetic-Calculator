#ifndef BLOCKS_CALCULATOR_H
#define BLOCKS_CALCULATOR_H

#include <optional>
#include <unordered_set>

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/logic/PortsConnectionSide.h"
#include "GUI/logic/errors/GraphCycleException.h"
#include "delegate/IBlocksRegistryDelegate.h"
#include "logging/Loggable.h"

namespace gui::logic::calculations {

    class BlocksCalculator : public business_logic::Loggable<BlocksCalculator> {
       public:
        BlocksCalculator(
            gui::logic::calculations::delegate::IBlocksRegistryDelegate* blocksRegistryDelegate);

        /**
         * @brief Calculates the values flowing through the graph
         * @throws `gui::logic::errors::GraphCycleException` if a cycle is detected
         */
        void calculateValuesFlow();

       protected:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<BlocksCalculator>::logger;

        /**
         * @brief Calculates the values of the dependencies of a block and then the block itself
         */
        void calculateBlockValues(gui::elements::base::BaseBlock* block);

        /**
         * @brief The set of visited blocks to detect cycles
         */
        std::unordered_set<gui::logic::PortsConnectionSide> visited;

        /**
         * @brief The stack of recursion to detect cycles
         */
        std::unordered_set<gui::logic::PortsConnectionSide> recursionStack;

        /**
         * @brief The set of visited blocks to note which blocks have been visited when calculating
         * their values
         */
        std::unordered_set<gui::elements::base::BaseBlock*> calcVisited;

        /**
         * @brief Checks if there is a cycle in the graph
         * @param side The side to check for cycles
         * @return The cycle if found, otherwise std::nullopt
         */
        std::optional<std::unordered_set<gui::logic::PortsConnectionSide>> hasCycle(
            gui::logic::PortsConnectionSide side);

        /**
         * @brief The delegate that provides the connections registry
         */
        gui::logic::calculations::delegate::IBlocksRegistryDelegate* blocksRegistryDelegate;
    };

}  // namespace gui::logic::calculations
#endif  // BLOCKS_CALCULATOR_H
