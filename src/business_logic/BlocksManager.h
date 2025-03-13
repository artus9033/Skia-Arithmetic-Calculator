#ifndef BUSINESS_LOGIC_LOGIC_BLOCKS_MANAGER_H
#define BUSINESS_LOGIC_LOGIC_BLOCKS_MANAGER_H

#include <algorithm>
#include <chrono>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <unordered_set>
#include <vector>

#include "calculations/BlocksCalculator.h"
#include "calculations/delegate/IBlocksRegistryDelegate.h"
#include "components/UIText.h"
#include "components/UITextsRow.h"
#include "constants.h"
#include "delegate/IBlockLifecycleManagerDelegate.h"
#include "delegate/INewBlockChoiceDelegate.h"
#include "delegate/IWindowDelegate.h"
#include "elements/blocks/BaseBlock.h"
#include "elements/blocks/BlocksRegistry.h"
#include "elements/blocks/structures/BlocksConnectionSide.h"
#include "errors/GraphCycleException.h"
#include "geometry/Size2D.h"
#include "input/ConnectPortsInteraction.h"
#include "input/InputChoiceInteraction.h"
#include "logging/Loggable.h"

namespace business_logic {
    /**
     * @brief Manages the blocks in the GUI, both their rendering and interaction
     */

    class BlocksManager : public business_logic::delegate::INewBlockChoiceDelegate,
                          public business_logic::delegate::IBlockLifecycleManagerDelegate,
                          public business_logic::calculations::delegate::IBlocksRegistryDelegate,
                          public business_logic::calculations::BlocksCalculator,
                          public business_logic::Loggable<BlocksManager> {
       public:
        explicit BlocksManager(business_logic::delegate::IWindowDelegate* windowDelegate);

        ~BlocksManager() noexcept override = default;

        // disable copy semantics
        BlocksManager(const BlocksManager&) = delete;
        BlocksManager& operator=(const BlocksManager&) = delete;

        // disable move semantics
        BlocksManager(BlocksManager&&) = delete;
        BlocksManager& operator=(BlocksManager&&) = delete;

        /**
         * @brief Handles the mouse down event
         */
        void handleMouseDown();

        /**
         * @brief Handles the mouse up event
         */
        void handleMouseUp();

        /**
         * @brief Handles the mouse move event
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        void handleMouseMove(int x, int y);

        /**
         * @brief Handles the numeric key press event
         * @param number The number that was pressed
         */
        void handleNumericKeyPress(int number);

        /**
         * @brief Handles the ESC key press event
         */
        void handleEscapeKeyPress();

        /**
         * @brief Gets the block at the mouse position
         * @return The block at the mouse position, or `std::nullopt` if no block is at the mouse
         * position
         */
        std::optional<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>
        getBlockAtMousePos();

        /**
         * @brief Gets the block at the given coordinates
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         * @return The block at the given coordinates, or `std::nullopt` if no block is at the given
         * coordinates
         */
        std::optional<std::shared_ptr<business_logic::elements::blocks::BaseBlock>> getBlockAt(
            int x, int y);

        /**
         * @brief Called when a new block is chosen to be added to the canvas
         * @param blockType The type of the block that was chosen
         */
        void onNewBlockChoice(
            const business_logic::elements::blocks::BlockType& blockType) override;

        /**
         * @brief Sets the input callback; this will override rendering other entities
         * @param inputChoiceInteraction The input choice interaction
         */
        void setActiveChoicesInput(
            business_logic::input::InputChoiceInteraction<
                business_logic::elements::blocks::BlockType>&& inputChoiceInteraction);

        /**
         * @brief Checks if the input callback is active
         * @return True if the input callback is active, false otherwise
         */
        bool hasActiveChoicesInput() const;

        /**
         * @brief Clears the active input choices and the corresponding generated UI texts rows
         */
        void clearActiveChoicesInput();

        /**
         * \copydoc business_logic::delegate::IBlockLifecycleManagerDelegate::onPortsConnected
         */
        void onPortsConnected(
            const business_logic::elements::structures::BlocksConnectionSide& source,
            const business_logic::elements::structures::BlocksConnectionSide& dest) override;

        /**
         * \copydoc business_logic::delegate::IBlockLifecycleManagerDelegate::hasConnectionBetween
         */
        bool hasConnectionBetween(
            const business_logic::elements::structures::BlocksConnectionSide& source,
            const business_logic::elements::structures::BlocksConnectionSide& dest) const override;

        /**
         * \copydoc business_logic::delegate::IBlockLifecycleManagerDelegate::isInputConnected
         */
        bool isInputConnected(
            const business_logic::elements::structures::BlocksConnectionSide& side) const override;

        /**
         * \copydoc business_logic::delegate::IBlockLifecycleManagerDelegate::onBlockDeleted
         */
        void onBlockDeleted(const business_logic::elements::blocks::BaseBlock* block) override;

        /**
         * @brief Handles right click event on a block
         * @param block The block that was right-clicked
         */
        void handleRightClickOnBlock(
            const std::shared_ptr<business_logic::elements::blocks::BaseBlock>& block);

        /**
         * \copydoc business_logic::calculations::delegate::IBlocksRegistryDelegate::getBlocks
         */
        const std::vector<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>& getBlocks()
            const override;

        /**
         * \copydoc \
         * business_logic::calculations::delegate::IBlocksRegistryDelegate::getConnectionsRegistry
         */
        const std::unordered_map<
            business_logic::elements::structures::BlocksConnectionSide,
            std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>>&
        getConnectionsRegistry() const override;

       protected:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<BlocksManager>::logger;

        /**
         * The blocks in the manager
         */
        std::vector<std::shared_ptr<business_logic::elements::blocks::BaseBlock>> blocks;

        /**
         * The block that is currently being dragged
         */
        business_logic::elements::blocks::BaseBlock* draggedBlock;

        /**
         * The mouse X coordinate
         */
        int mouseX;

        /**
         * The mouse Y coordinate
         */
        int mouseY;

        /**
         * The offset from the block's center when dragging started
         */
        business_logic::geometry::Size2D dragOffset;

        /**
         * The time of the last mouse click, used for tracking double-clicks
         */
        std::chrono::steady_clock::time_point doubleClickCtLastMouseClickTime;

        /**
         * The window delegate
         */
        business_logic::delegate::IWindowDelegate* windowDelegate;

        /**
         * The input choice interaction
         */
        std::optional<business_logic::input::InputChoiceInteraction<
            business_logic::elements::blocks::BlockType>>
            inputChoiceInteraction;

        /**
         * The connect ports interaction singleton
         */
        business_logic::input::ConnectPortsInteraction connectPortsInteraction;

        /**
         * The UI texts for the choices; prepared inside `setActiveChoicesInput(...)` and
         * used in `render(...)`
         */
        std::vector<components::UITextsRow> inputChoicesUiTextsRows;

        /**
         * The blocks registry
         */
        std::unordered_map<
            business_logic::elements::structures::BlocksConnectionSide,
            std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>>
            connectionsRegistry;

        /**
         * Stores the cycle path (if present)
         */
        std::optional<
            std::unordered_set<business_logic::elements::structures::BlocksConnectionSide>>
            maybeGraphCycle;
    };

}  // namespace business_logic

#endif  // BUSINESS_LOGIC_LOGIC_BLOCKS_MANAGER_H
