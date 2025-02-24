#ifndef GUI_WINDOW_WINDOW_BASE_H
#define GUI_WINDOW_WINDOW_BASE_H

#include <memory>

#include "GUI/elements/BlocksRegistry.h"
#include "GUI/input/InputChoiceInteraction.h"
#include "GUI/logic/BlocksManager.h"
#include "spdlog/spdlog.h"

namespace gui::window {
    class WindowBase {
       public:
        WindowBase(std::shared_ptr<spdlog::logger> logger)
            : blocksManager(std::make_shared<gui::logic::BlocksManager>()), logger(logger) {}

        virtual ~WindowBase() = default;

        /**
         * @brief Runs the main window loop
         */
        virtual void run() = 0;

        /**
         * @brief Whether the window should close
         * @return bool True if the window should close, false otherwise
         */
        virtual bool shouldClose() const = 0;

        /**
         * @brief Handles the mouse down event; should be called internally by the window
         * implementation
         */
        void handleMouseDown() { blocksManager->handleMouseDown(); }

        /**
         * @brief Handles the mouse up event; should be called internally by the window
         * implementation
         */
        void handleMouseUp() { blocksManager->handleMouseUp(); }

        /**
         * @brief Handles the numeric key press event; should be called internally by the window
         * implementation
         */
        void handleNumericKeyPress(int number) { blocksManager->handleNumericKeyPress(number); }

        /**
         * @brief Handles the ESC key press event; should be called internally by the window
         * implementation
         */
        void handleEscapeKeyPress() { blocksManager->handleEscapeKeyPress(); }

        /**
         * @brief Handles the right click event; should be called internally by the window
         * implementation
         */
        void handleRightClick() {
            auto maybeHoveredBlock = blocksManager->getBlockAtMousePos();

            // if there is no hovered block, open the add block popup
            if (!maybeHoveredBlock.has_value()) {
                if (blocksManager->hasActiveChoicesInput()) {
                    logger->info(
                        "Right clicked on empty space, but input is already active, ignoring "
                        "event");
                } else {
                    logger->info("Right clicked on empty space, opening add block popup");

                    this->blocksManager->setActiveChoicesInput({
                        .choices = gui::elements::base::BlockInputChoices,
                        .callback =
                            [this](const gui::elements::base::BlockType& blockType) {
                                this->blocksManager->onNewBlockChoice(blockType);
                            },
                    });
                }
            }
        }

        /**
         * @brief Handles the mouse move event; should be called internally by the window
         * implementation
         */
        void handleMouseMove(int x, int y) { blocksManager->handleMouseMove(x, y); }

       protected:
        /**
         * The blocks manager, this class will pass it the interaction events
         */
        std::shared_ptr<gui::logic::BlocksManager> blocksManager;

        /**
         * The logger for the window
         */
        std::shared_ptr<spdlog::logger> logger;
    };
}  // namespace gui::window

#endif  // GUI_WINDOW_WINDOW_BASE_H
