#ifndef GUI_ELEMENTS_BASE_BLOCK_RENDERER_H
#define GUI_ELEMENTS_BASE_BLOCK_RENDERER_H

#include <skia/include/core/SkCanvas.h>

#include <magic_enum/magic_enum.hpp>

#include "business_logic/components/UIText.h"
#include "business_logic/constants.h"
#include "business_logic/elements/blocks/BaseBlock.h"
#include "renderer/FontManager.h"
#include "renderer/colors.h"

/**
 * @brief The GUI counterpart carrying implementations integrating with business_logic::elements
 */
namespace gui::elements {

    // using an empty namespace to avoid polluting the outer scope of business_logic::elements::base
    namespace {
        namespace colors = gui::renderer::colors;
        namespace components = business_logic::components;
    }  // namespace

    /**
     * @brief A Skia-backed renderer implementation for a
     * business_logic::elements::blocks::BaseBlock
     */
    class SkiaBaseBlockRenderer {
       public:
        SkiaBaseBlockRenderer() = delete;
        virtual ~SkiaBaseBlockRenderer() = delete;

        // disable copy semantics
        SkiaBaseBlockRenderer(const SkiaBaseBlockRenderer&) = delete;
        SkiaBaseBlockRenderer& operator=(const SkiaBaseBlockRenderer&) = delete;

        // disable move semantics
        SkiaBaseBlockRenderer(SkiaBaseBlockRenderer&&) = delete;
        SkiaBaseBlockRenderer& operator=(SkiaBaseBlockRenderer&&) = delete;

        /**
         * @brief Renders a block on an SkCanvas
         *
         * @param block The block to render
         * @param canvas The canvas to render the block on
         * @param mouseX The x coordinate of the mouse
         * @param mouseY The y coordinate of the mouse
         * @param isHovered Whether the block is hovered
         *
         * @see business_logic::elements::blocks::BaseBlock::render
         */
        static void render(business_logic::elements::blocks::BaseBlock* block,
                           SkCanvas* canvas,
                           int mouseX,
                           int mouseY,
                           bool isHovered);

       private:
        /**
         * @brief Renders the value above the block
         *
         * @param block The block to render the value above
         * @param canvas The canvas to render the value above the block on
         * @param isHovered Whether the block is hovered
         * @param blockValue The value to render above the block
         */
        static void renderValueAboveBlock(const business_logic::elements::blocks::BaseBlock* block,
                                          SkCanvas* canvas,
                                          bool isHovered,
                                          const std::string& blockValue);
    };
}  // namespace gui::elements

#endif  // GUI_ELEMENTS_BASE_BLOCK_RENDERER_H
