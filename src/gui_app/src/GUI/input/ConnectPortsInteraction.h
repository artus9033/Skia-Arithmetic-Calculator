#ifndef GUI_INPUT_CONNECT_PORTS_INTERACTION_H
#define GUI_INPUT_CONNECT_PORTS_INTERACTION_H

#include <functional>
#include <vector>

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/elements/base/Port.h"
#include "GUI/logic/MessageBox.h"
#include "GUI/logic/PortsConnectionSide.h"
#include "logging/Loggable.h"

namespace gui::input {
    /**
     * @brief Connect ports via dragging interaction DTO
     */
    class ConnectPortsInteraction : public business_logic::Loggable<ConnectPortsInteraction> {
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

        void handleUserInteractedWith(
            gui::elements::base::BaseBlock* block,
            const gui::elements::base::Port* port,
            gui::window::delegate::IWindowDelegate* windowDelegate,
            gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate) {
            if (isStarted()) {
                logger->info(
                    "Interaction handling new event; it was already started, completing it");

                // complete the interaction
                endSide.emplace(gui::logic::PortsConnectionSide{.block = block, .port = port});

                // sanitization: the concept is to be sure that the start side is the output port
                // and the end side is the input port
                if (startSide.value().port->type == gui::elements::base::Port::Type::INPUT) {
                    std::swap(startSide, endSide);
                }

                // validate if the connection is valid
                if (startSide.value().port->type == gui::elements::base::Port::Type::OUTPUT &&
                    endSide.value().port->type == gui::elements::base::Port::Type::INPUT) {
                    if (startSide.value().block == endSide.value().block) {
                        logger->warn("Connection is invalid: start and end blocks are the same");

                        gui::logic::MessageBox::showWarning(
                            "Invalid connection",
                            "This connection is invalid. A valid connection must "
                            "be between an input and output port of different blocks.",
                            windowDelegate);
                    } else {
                        // validate if the connection does not already exist
                        if (blockLifecycleManagerDelegate->hasConnectionBetween(startSide.value(),
                                                                                endSide.value())) {
                            logger->warn("Connection is invalid: connection already exists");

                            gui::logic::MessageBox::showWarning("Invalid connection",
                                                                "Such a connection already exists.",
                                                                windowDelegate);
                        } else {
                            // validate if the input port is not already connected to something
                            if (blockLifecycleManagerDelegate->isInputConnected(endSide.value())) {
                                logger->warn(
                                    "Connection is invalid: input port is already connected to "
                                    "something");

                                gui::logic::MessageBox::showWarning(
                                    "Invalid connection",
                                    "The chosen input port is already connected.",
                                    windowDelegate);
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

                    gui::logic::MessageBox::showWarning(
                        "Invalid connection",
                        "This connection is invalid. A valid connection must "
                        "be between an input and output port (the order is "
                        "not important).",
                        windowDelegate);
                }

                // reset the interaction
                resetInteraction();
            } else {
                logger->info("Interaction handling new event; it was not started, starting it");

                // start the interaction
                startSide.emplace(gui::logic::PortsConnectionSide{.block = block, .port = port});
            }
        }

        /**
         * @brief Gets an immutable reference to the start side
         */
        const gui::logic::PortsConnectionSide& getStartSide() const { return startSide.value(); }

        /**
         * @brief Gets an immutable reference to the end side
         */
        const gui::logic::PortsConnectionSide& getEndSide() const { return endSide.value(); }

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
        std::optional<gui::logic::PortsConnectionSide> startSide;

        /**
         * @brief the second interaction side
         */
        std::optional<gui::logic::PortsConnectionSide> endSide;

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
}  // namespace gui::input

#endif  // GUI_INPUT_CONNECT_PORTS_INTERACTION_H
