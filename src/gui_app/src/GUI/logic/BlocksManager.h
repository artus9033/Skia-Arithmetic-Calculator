#ifndef GUI_LOGIC_BLOCKS_MANAGER_H
#define GUI_LOGIC_BLOCKS_MANAGER_H

#include <QMessageBox>
#include <algorithm>
#include <boost/range/adaptor/reversed.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "GUI/elements/BlocksRegistry.h"
#include "GUI/elements/base/BaseBlock.h"
#include "GUI/elements/impl/ConstantBlock.h"
#include "GUI/elements/impl/MonitorBlock.h"
#include "GUI/geometry/Size2D.h"
#include "GUI/input/ConnectPortsInteraction.h"
#include "GUI/input/InputChoiceInteraction.h"
#include "GUI/logic/PortsConnectionSide.h"
#include "GUI/renderer/colors.h"
#include "GUI/renderer/delegate/UIRendererDelegate.h"
#include "GUI/window/delegate/IWindowDelegate.h"
#include "MessageBox.h"
#include "constants.h"
#include "delegate/IBlockLifecycleManagerDelegate.h"
#include "delegate/INewBlockChoiceDelegate.h"
#include "logging/Loggable.h"
#include "magic_enum/magic_enum.hpp"

#define MAX_INPUT_CHOICES_PER_ROW 3

namespace gui::logic {
    // using an empty namespace to avoid polluting the outer scope of gui::logic
    namespace {
        namespace components = gui::renderer::components;
        namespace colors = gui::renderer::colors;
    }  // namespace

    /**
     * @brief Manages the blocks in the GUI, both their rendering and interaction
     */
    class BlocksManager : public gui::logic::delegate::INewBlockChoiceDelegate,
                          public gui::logic::delegate::IBlockLifecycleManagerDelegate,
                          public business_logic::Loggable<BlocksManager> {
       public:
        BlocksManager(gui::window::delegate::IWindowDelegate* windowDelegate);

        /**
         * @brief Handles the mouse down event
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         */
        void handleMouseDown();

        /**
         * @brief Handles the mouse up event
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
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
         * @brief Renders the blocks on the canvas
         * @param canvas SkCanvas to draw on
         */
        void render(SkCanvas* canvas,
                    const geometry::Size2D& size,
                    gui::renderer::delegate::UIRendererDelegate*& uiRendererDelegate);

        /**
         * @brief Gets the block at the mouse position
         * @return The block at the mouse position, or `std::nullopt` if no block is at the mouse
         * position
         */
        std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> getBlockAtMousePos();

        /**
         * @brief Gets the block at the given coordinates
         * @param x X coordinate of the mouse
         * @param y Y coordinate of the mouse
         * @return The block at the given coordinates, or `std::nullopt` if no block is at the given
         * coordinates
         */
        std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> getBlockAt(int x, int y);

        /**
         * @brief Called when a new block is chosen to be added to the canvas
         * @param blockType The type of the block that was chosen
         */
        void onNewBlockChoice(const gui::elements::base::BlockType& blockType) override;

        /**
         * @brief Sets the input callback; this will override rendering other entities
         * @param inputChoiceInteraction The input choice interaction
         */
        void setActiveChoicesInput(
            gui::input::InputChoiceInteraction<gui::elements::base::BlockType>&&
                inputChoiceInteraction);

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
         * \copydoc gui::logic::delegate::IBlockLifecycleManagerDelegate::onPortsConnected
         */
        void onPortsConnected(const gui::logic::PortsConnectionSide& source,
                              const gui::logic::PortsConnectionSide& dest) override;

        /**
         * \copydoc gui::logic::delegate::IBlockLifecycleManagerDelegate::hasConnectionBetween
         */
        bool hasConnectionBetween(const gui::logic::PortsConnectionSide& source,
                                  const gui::logic::PortsConnectionSide& dest) const override;

        /**
         * \copydoc gui::logic::delegate::IBlockLifecycleManagerDelegate::onBlockDeleted
         */
        void onBlockDeleted(const gui::elements::base::BaseBlock* block) override;

        /**
         * @brief Handles right click event on a block
         * @param block The block that was right-clicked
         */
        void handleRightClickOnBlock(const std::shared_ptr<gui::elements::base::BaseBlock>& block);

       protected:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<BlocksManager>::logger;

        /**
         * The blocks in the manager
         */
        std::vector<std::shared_ptr<gui::elements::base::BaseBlock>> blocks;

        /**
         * The block that is currently being dragged
         */
        std::shared_ptr<gui::elements::base::BaseBlock> draggedBlock;

        /**
         * The block that is currently being hovered over
         */
        std::shared_ptr<gui::elements::base::BaseBlock> hoveredBlock;

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
        geometry::Size2D dragOffset;

        /**
         * The time of the last mouse click
         */
        time_t lastMouseClickTime;

        /**
         * The window delegate
         */
        gui::window::delegate::IWindowDelegate* windowDelegate;

        /**
         * @brief Renders the dragged line on the canvas
         * @param canvas SkCanvas to draw on
         */
        void maybeRenderDraggedLine(SkCanvas* canvas);

        /**
         * @brief The input choice interaction
         */
        std::optional<gui::input::InputChoiceInteraction<gui::elements::base::BlockType>>
            inputChoiceInteraction;

        /**
         * @brief The connect ports interaction singleton
         */
        gui::input::ConnectPortsInteraction connectPortsInteraction;

        /**
         * @brief The UI texts for the choices; prepared inside `setActiveChoicesInput(...)` and
         * used in `render(...)`
         */
        std::vector<gui::renderer::components::UITextsRow> inputChoicesUiTextsRows;

        /**
         * @brief The blocks registry
         */
        std::unordered_map<gui::logic::PortsConnectionSide, gui::logic::PortsConnectionSide>
            connectionsRegistry;

        /**
         * @brief The paint for the connector lines
         */
        static SkPaint connectorPaint;
    };

}  // namespace gui::logic

#endif  // GUI_LOGIC_BLOCKS_MANAGER_H
