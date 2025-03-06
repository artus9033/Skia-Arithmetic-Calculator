#ifndef BLOCKS_CALCULATOR_H
#define BLOCKS_CALCULATOR_H

#include <optional>
#include <unordered_set>

#include "delegate/IBlocksRegistryDelegate.h"
#include "elements/blocks/BaseBlock.h"
#include "elements/blocks/structures/BlocksConnectionSide.h"
#include "errors/GraphCycleException.h"
#include "logging/Loggable.h"

namespace business_logic::calculations {
    /**
     * @brief The blocks calculator logic, computes the flowing values
     */
    class BlocksCalculator : public business_logic::Loggable<BlocksCalculator> {
       public:
        BlocksCalculator(business_logic::calculations::delegate::IBlocksRegistryDelegate*
                             blocksRegistryDelegate);

        /**
         * @brief Calculates the values flowing through the graph
         * @throws `business_logic::errors::GraphCycleException` if a cycle is detected
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
        void calculateBlockValues(business_logic::elements::blocks::BaseBlock* block);

        /**
         * @brief The set of visited blocks to detect cycles
         */
        std::unordered_set<business_logic::elements::structures::BlocksConnectionSide> visited;

        /**
         * @brief The stack of recursion to detect cycles
         */
        std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>
            recursionStack;

        /**
         * @brief The set of visited blocks to note which blocks have been visited when calculating
         * their values
         */
        std::unordered_set<business_logic::elements::blocks::BaseBlock*> calcVisited;

        /**
         * @brief Checks if there is a cycle in the graph
         * @param side The side to check for cycles
         * @return The cycle if found, otherwise std::nullopt
         */
        std::optional<
            std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>>
        hasCycle(elements::structures::BlocksConnectionSide side);

        /**
         * @brief The delegate that provides the connections registry
         */
        business_logic::calculations::delegate::IBlocksRegistryDelegate* blocksRegistryDelegate;
    };

}  // namespace business_logic::calculations
#endif  // BLOCKS_CALCULATOR_H
