#ifndef GUI_WINDOW_WINDOW_BASE_H
#define GUI_WINDOW_WINDOW_BASE_H

#include <memory>

#include "business_logic/BlocksManager.h"
#include "business_logic/elements/blocks/BlocksRegistry.h"
#include "delegate/IWindowDelegate.h"
#include "elements/SkiaBlocksManagerRenderer.h"
#include "input/InputChoiceInteraction.h"
#include "spdlog/spdlog.h"

/**
 * @brief The GUI window abstraction & implementation
 */
namespace gui::window {
    /**
     * @brief The abstract base class for implementing a window
     *
     * @see business_logic::delegate::IWindowDelegate
     *
     * @tparam Canvas The type of the canvas
     */
    template <typename Canvas>
    class WindowBase : public business_logic::delegate::IWindowDelegate {
       public:
        WindowBase(std::shared_ptr<spdlog::logger> logger)
            : blocksManager(std::make_shared<gui::elements::SkiaBlocksManagerRenderer>(this)),
              logger(logger) {}

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

            if (maybeHoveredBlock.has_value()) {
                blocksManager->handleRightClickOnBlock(maybeHoveredBlock.value());
            } else {
                // if there is no hovered block, open the add block popup
                if (blocksManager->hasActiveChoicesInput()) {
                    logger->info(
                        "Right clicked on empty space, but input is already active, ignoring "
                        "event");
                } else {
                    logger->info("Right clicked on empty space, opening add block popup");

                    std::vector<business_logic::input::InputChoice<
                        business_logic::elements::blocks::BlockType>>
                        choices = {};

                    for (const auto& choice :
                         magic_enum::enum_values<business_logic::elements::blocks::BlockType>()) {
                        choices.push_back(business_logic::input::InputChoice<
                                          business_logic::elements::blocks::BlockType>{
                            .displayName = std::string(magic_enum::enum_name(choice)),
                            .value = choice,
                        });
                    }

                    this->blocksManager->setActiveChoicesInput({
                        .choices = choices,
                        .callback =
                            [this](const business_logic::elements::blocks::BlockType& blockType) {
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

        /**
         * \copydoc business_logic::delegate::IWindowDelegate::getWindowSize
         */
        business_logic::geometry::Size2D getWindowSize() { return winSize; }

        /**
         * \copydoc business_logic::delegate::IWindowDelegate::getFramebufferSize
         */
        business_logic::geometry::Size2D getFramebufferSize() { return framebufferSize; }

       protected:
        /**
         * The blocks manager, this class will pass it the interaction events
         */
        std::shared_ptr<business_logic::BlocksManager> blocksManager;

        /**
         * The logger for the window
         */
        std::shared_ptr<spdlog::logger> logger;

        /**
         * The window size
         */
        business_logic::geometry::Size2D winSize;

        /**
         * The framebuffer size
         */
        business_logic::geometry::Size2D framebufferSize;
    };
}  // namespace gui::window

#endif  // GUI_WINDOW_WINDOW_BASE_H
