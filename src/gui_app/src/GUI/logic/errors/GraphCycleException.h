#ifndef GUI_LOGIC_ERRORS_GRAPH_CYCLE_EXCEPTION_H
#define GUI_LOGIC_ERRORS_GRAPH_CYCLE_EXCEPTION_H

#include <stdexcept>
#include <unordered_set>

#include "GUI/logic/PortsConnectionSide.h"

namespace gui::logic::errors {
    /**
     * @brief Exception thrown when a graph cycle is detected
     */
    class GraphCycleException : public std::runtime_error {
       public:
        GraphCycleException(const std::unordered_set<gui::logic::PortsConnectionSide>& cyclePath)
            : std::runtime_error("Graph cycle detected"), cyclePath(cyclePath) {}

        /**
         * @brief Gets the cycle path
         * @return The cycle path
         */
        const std::unordered_set<gui::logic::PortsConnectionSide>& getCyclePath() const {
            return cyclePath;
        }

       private:
        std::unordered_set<gui::logic::PortsConnectionSide> cyclePath;
    };
}  // namespace gui::logic::errors

#endif  // GUI_LOGIC_ERRORS_GRAPH_CYCLE_EXCEPTION_H
