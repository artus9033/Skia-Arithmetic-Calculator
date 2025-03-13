#ifndef BUSINESS_LOGIC_LOGIC_ERRORS_GRAPH_CYCLE_EXCEPTION_H
#define BUSINESS_LOGIC_LOGIC_ERRORS_GRAPH_CYCLE_EXCEPTION_H

#include <stdexcept>
#include <unordered_set>

#include "elements/blocks/structures/BlocksConnectionSide.h"

/**
 * @brief Logic exception classes
 */
namespace business_logic::errors {
    /**
     * @brief Exception thrown when a graph cycle is detected
     */
    class GraphCycleException : public std::runtime_error {
       public:
        explicit GraphCycleException(
            const std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>&
                cyclePath)
            : std::runtime_error("Graph cycle detected"), cyclePath(cyclePath) {}

        ~GraphCycleException() noexcept override = default;

        /**
         * @brief Gets the cycle path
         * @return The cycle path
         */
        const std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>&
        getCyclePath() const {
            return cyclePath;
        }

       private:
        std::unordered_set<business_logic::elements::structures::BlocksConnectionSide> cyclePath;
    };
}  // namespace business_logic::errors

#endif  // BUSINESS_LOGIC_LOGIC_ERRORS_GRAPH_CYCLE_EXCEPTION_H
