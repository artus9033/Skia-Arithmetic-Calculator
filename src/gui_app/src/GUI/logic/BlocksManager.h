#ifndef GUI_LOGIC_BLOCKS_MANAGER_H
#define GUI_LOGIC_BLOCKS_MANAGER_H

#include <algorithm>
#include <boost/range/adaptor/reversed.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "GUI/elements/BlocksRegistry.h"
#include "GUI/elements/base/BaseBlock.h"
#include "GUI/geometry/Size2D.h"
#include "GUI/input/InputChoiceInteraction.h"
#include "GUI/renderer/delegate/UIRendererDelegate.h"
#include "constants.h"
#include "delegate/INewBlockChoiceDelegate.h"
#include "logging/Loggable.h"

// GCC-specific demangling API
#ifdef __GNUC__
#include <cxxabi.h>

#include <cstdlib>
#endif

#define MAX_INPUT_CHOICES_PER_ROW 3

namespace gui::logic {
    /**
     * @brief Manages the blocks in the GUI, both their rendering and interaction
     */
    class BlocksManager : public gui::logic::delegate::INewBlockChoiceDelegate,
                          public business_logic::Loggable<BlocksManager> {
       public:
        BlocksManager();

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
         * The time of the last mouse click
         */
        time_t lastMouseClickTime;

        /**
         * @brief Renders the dragged line on the canvas
         * @param canvas SkCanvas to draw on
         */
        void maybeRenderDraggedLine(SkCanvas* canvas) const;

        /**
         * @brief The input choice interaction
         */
        std::optional<gui::input::InputChoiceInteraction<gui::elements::base::BlockType>>
            inputChoiceInteraction;

        /**
         * @brief The UI renderer delegate
         */
        gui::renderer::delegate::UIRendererDelegate* uiRendererDelegate;

        /**
         * @brief The UI texts for the choices; prepared inside `setActiveChoicesInput(...)` and
         * used in `render(...)`
         */
        std::vector<gui::renderer::components::UITextsRow> inputChoicesUiTextsRows;
    };

}  // namespace gui::logic

#endif  // GUI_LOGIC_BLOCKS_MANAGER_H
