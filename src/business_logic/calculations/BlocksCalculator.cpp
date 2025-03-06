#include "BlocksCalculator.h"

namespace business_logic::calculations {
    BlocksCalculator::BlocksCalculator(
        business_logic::calculations::delegate::IBlocksRegistryDelegate* blocksRegistryDelegate)
        : blocksRegistryDelegate(blocksRegistryDelegate) {}

    void BlocksCalculator::calculateValuesFlow() {
        auto blocks = blocksRegistryDelegate->getBlocks();

        visited.clear();
        recursionStack.clear();

        // check for cycles first
        for (const auto& block : blocks) {
            auto maybeCycle = hasCycle({.block = block.get(), .port = nullptr});

            if (maybeCycle) {
                throw errors::GraphCycleException(maybeCycle.value());
            }
        }

        // calculate values for all blocks
        calcVisited.clear();
        for (const auto& block : blocks) {
            calculateBlockValues(block.get());
        }
    }

    void BlocksCalculator::calculateBlockValues(
        business_logic::elements::blocks::BaseBlock* block) {
        if (calcVisited.find(block) != calcVisited.end()) {
            return;
        }

        // mark as visited to prevent cycles
        calcVisited.insert(block);

        // first calculate values for all input blocks
        for (const auto& [source, destinations] :
             blocksRegistryDelegate->getConnectionsRegistry()) {
            const auto& sourcePortValue = source.block->getPortValue(source.port);

            for (const auto& dest : destinations) {
                if (dest.block == block) {
                    calculateBlockValues(source.block);

                    // copy value from source output to destination input
                    block->setPortValue(dest.port, sourcePortValue);
                }
            }
        }

        // now calculate this block's output values
        block->calculateOutputValues();
    }

    std::optional<std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>>
    BlocksCalculator::hasCycle(elements::structures::BlocksConnectionSide side) {
        if (std::any_of(recursionStack.begin(), recursionStack.end(), [side](const auto& predSide) {
                return side.block == predSide.block;
            })) {
            // cycle found
            std::unordered_set<business_logic::elements::structures::BlocksConnectionSide> cycle = {
                {.block = side.block, .port = nullptr}};

            for (const auto& side : visited) {
                cycle.insert(side);
            }

            for (const auto& side : recursionStack) {
                cycle.insert(side);
            }

            return cycle;
        }

        if (visited.contains(side)) {
            // block already processed
            return std::nullopt;
        }

        // visit the block
        visited.insert(side);
        recursionStack.insert(side);

        // check all output connections of the block
        for (const auto& [source, destinations] :
             blocksRegistryDelegate->getConnectionsRegistry()) {
            if (source.block == side.block) {
                for (const auto& dest : destinations) {
                    auto maybeCycle = hasCycle(dest);
                    if (maybeCycle.has_value()) {
                        auto cycle = maybeCycle.value();

                        for (const auto& side : visited) {
                            cycle.insert(side);
                        }

                        for (const auto& side : recursionStack) {
                            cycle.insert(side);
                        }

                        cycle.insert(dest);

                        return cycle;
                    }
                }
            }
        }

        recursionStack.erase(side);
        return std::nullopt;
    }

}  // namespace business_logic::calculations
