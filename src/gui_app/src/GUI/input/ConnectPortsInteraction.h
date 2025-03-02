#ifndef GUI_INPUT_CONNECT_PORTS_INTERACTION_H
#define GUI_INPUT_CONNECT_PORTS_INTERACTION_H

#include <functional>
#include <vector>

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/logic/MessageBox.h"
#include "InputChoice.h"
#include "logging/Loggable.h"

namespace gui::input {
    struct PortsConnectionHolder {
        gui::elements::base::BaseBlock* block;
        const gui::elements::base::Port* port;
    };

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

        void handleUserInteractedWith(gui::elements::base::BaseBlock* block,
                                      const gui::elements::base::Port* port,
                                      gui::window::delegate::IWindowDelegate* windowDelegate) {
            if (isStarted()) {
                logger->info(
                    "Interaction handling new event; it was already started, completing it");

                // complete the interaction
                endSide.emplace(PortsConnectionHolder{.block = block, .port = port});

                // sanitization: the concept is to be sure that the start side is the input port
                // and the end side is the output port
                if (startSide.value().port->type == gui::elements::base::Port::Type::OUTPUT) {
                    std::swap(startSide, endSide);
                }

                // validate if the connection is valid
                if (startSide.value().port->type == gui::elements::base::Port::Type::INPUT &&
                    endSide.value().port->type == gui::elements::base::Port::Type::OUTPUT) {
                    // TODO: handle the connection
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
                startSide.emplace(PortsConnectionHolder{.block = block, .port = port});
            }
        }

        /**
         * @brief Gets an immutable reference to the start side
         */
        const PortsConnectionHolder& getStartSide() const { return startSide.value(); }

        /**
         * @brief Gets an immutable reference to the end side
         */
        const PortsConnectionHolder& getEndSide() const { return endSide.value(); }

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
        std::optional<PortsConnectionHolder> startSide;

        /**
         * @brief the second interaction side
         */
        std::optional<PortsConnectionHolder> endSide;

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
