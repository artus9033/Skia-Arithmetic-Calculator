#ifndef BUSINESS_LOGIC_INPUT_CONNECT_PORTS_INTERACTION_H
#define BUSINESS_LOGIC_INPUT_CONNECT_PORTS_INTERACTION_H

#include <functional>
#include <vector>

#include "elements/blocks/BaseBlock.h"
#include "elements/blocks/structures/BlocksConnectionSide.h"
#include "elements/blocks/structures/Port.h"
#include "logging/Loggable.h"

/**
 * @brief Input primitives, either for GUI (models of components) or interaction with elements
 */
namespace business_logic::input {
    /**
     * @brief Connect ports via dragging interaction DTO
     */
    class ConnectPortsInteraction final : public business_logic::Loggable<ConnectPortsInteraction> {
       public:
        /**
         * @brief Returns whether the interaction has started (i.e., is pending and a dragging line
         * should be rendered)
         */
        bool isStarted() const { return startSide.has_value(); }

        /**
         * @brief Sanitizes the interaction by removing both sides if any of them has become
         * invalid
         */
        void sanitize() {
            if (isInvalid()) {
                logger->info("Interaction is invalid, resetting");

                resetInteraction();
            }
        }

        void handleUserInteractedWith(business_logic::elements::blocks::BaseBlock* block,
                                      const business_logic::elements::structures::Port* port,
                                      business_logic::delegate::IWindowDelegate* windowDelegate,
                                      business_logic::delegate::IBlockLifecycleManagerDelegate*
                                          blockLifecycleManagerDelegate) {
            if (isStarted()) {
                logger->info(
                    "Interaction handling new event; it was already started, completing it");

                // complete the interaction
                endSide.emplace(business_logic::elements::structures::BlocksConnectionSide{
                    .block = block, .port = port});

                // sanitization: the concept is to be sure that the start side is the output port
                // and the end side is the input port
                if (startSide.value().port->type ==
                    business_logic::elements::structures::Port::Type::INPUT) {
                    std::swap(startSide, endSide);
                }

                // validate if the connection is valid
                if (startSide.value().port->type ==
                        business_logic::elements::structures::Port::Type::OUTPUT &&
                    endSide.value().port->type ==
                        business_logic::elements::structures::Port::Type::INPUT) {
                    if (startSide.value().block == endSide.value().block) {
                        logger->warn("Connection is invalid: start and end blocks are the same");

                        windowDelegate->showWarning(
                            "Invalid connection",
                            "This connection is invalid. A valid connection must "
                            "be between an input and output port of different blocks.");
                    } else {
                        // validate if the connection does not already exist
                        if (blockLifecycleManagerDelegate->hasConnectionBetween(startSide.value(),
                                                                                endSide.value())) {
                            logger->warn("Connection is invalid: connection already exists");

                            windowDelegate->showWarning("Invalid connection",
                                                        "Such a connection already exists.");
                        } else {
                            // validate if the input port is not already connected to something
                            if (blockLifecycleManagerDelegate->isInputConnected(endSide.value())) {
                                logger->warn(
                                    "Connection is invalid: input port is already connected to "
                                    "something");

                                windowDelegate->showWarning(
                                    "Invalid connection",
                                    "The chosen input port is already connected.");
                            } else {
                                logger->info(
                                    "Connection created between port '{}' (block {}) and port '{}' "
                                    "(block "
                                    "{})",
                                    startSide.value().port->name,
                                    startSide.value().block->getSelfId(),
                                    endSide.value().port->name,
                                    endSide.value().block->getSelfId());

                                // add the connection to the connections registry
                                blockLifecycleManagerDelegate->onPortsConnected(startSide.value(),
                                                                                endSide.value());
                            }
                        }
                    }
                } else {
                    logger->warn(
                        "Invalid connection: start side and end side are not valid port types");

                    windowDelegate->showWarning(
                        "Invalid connection",
                        "This connection is invalid. A valid connection must "
                        "be between an input and output port (the order is "
                        "not important).");
                }

                // reset the interaction
                resetInteraction();
            } else {
                logger->info("Interaction handling new event; it was not started, starting it");

                // start the interaction
                startSide.emplace(business_logic::elements::structures::BlocksConnectionSide{
                    .block = block, .port = port});
            }
        }

        /**
         * @brief Gets an immutable reference to the start side
         */
        [[maybe_unused]] const business_logic::elements::structures::BlocksConnectionSide&
        getStartSide() const {
            return startSide.value();
        }

        /**
         * @brief Gets an immutable reference to the end side
         */
        [[maybe_unused]] const business_logic::elements::structures::BlocksConnectionSide&
        getEndSide() const {
            return endSide.value();
        }

        /**
         * @brief Resets the interaction, setting both sides to `std::nullopt`
         */
        void resetInteraction() {
            startSide = std::nullopt;
            endSide = std::nullopt;
        }

       protected:
        /**
         * @brief the first interaction side
         */
        std::optional<business_logic::elements::structures::BlocksConnectionSide> startSide;

        /**
         * @brief the second interaction side
         */
        std::optional<business_logic::elements::structures::BlocksConnectionSide> endSide;

        /**
         * @brief Returns whether the interaction is invalid (i.e., one of the sides is no longer
         * valid)
         */
        bool isInvalid() {
            // if side A was clicked but is no longer valid (i.e., block has been deleted),
            // the interaction becomes invalid
            if (startSide.has_value() && !startSide.value().block) {
                return true;
            }

            // if side B was clicked but is no longer valid (i.e., block has been deleted),
            // the interaction becomes invalid
            if (endSide.has_value() && !endSide.value().block) {
                return true;
            }

            return false;
        }
    };
}  // namespace business_logic::input

#endif  // BUSINESS_LOGIC_INPUT_CONNECT_PORTS_INTERACTION_H
