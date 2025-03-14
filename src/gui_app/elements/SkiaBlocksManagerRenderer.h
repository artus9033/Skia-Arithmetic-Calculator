#ifndef BLOCKS_MANAGER_RENDERER_H
#define BLOCKS_MANAGER_RENDERER_H

#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkPaint.h>

#include "SkiaBaseBlockRenderer.h"
#include "business_logic/BlocksManager.h"
#include "geometry/Size2D.h"
#include "renderer/colors.h"
#include "renderer/delegate/UIRendererDelegate.h"

namespace gui::elements {
    // using an empty namespace to avoid polluting the outer scope of business_logic::elements::base
    namespace {
        namespace colors = gui::renderer::colors;
        namespace components = business_logic::components;
    }  // namespace

    /**
     * @brief Class that extends `business_logic::BlocksManager` and adds rendering logic
     */
    class SkiaBlocksManagerRenderer : public business_logic::BlocksManager {
       public:
        explicit SkiaBlocksManagerRenderer(
            business_logic::delegate::IWindowDelegate* windowDelegate)
            : business_logic::BlocksManager(windowDelegate) {}

        /**
         * @brief Renders the blocks on the canvas
         * @param canvas Canvas to draw on
         * @param size The size of the canvas
         * @param uiRendererDelegate The renderer delegate to render the blocks
         */
        void render(SkCanvas* canvas,
                    const business_logic::geometry::Size2D& size,
                    gui::renderer::delegate::UIRendererDelegate<SkCanvas>*& uiRendererDelegate);

        /**
         * @brief Renders the dragged line on the canvas
         * @param canvas Canvas to draw on
         */
        void maybeRenderDraggedLine(SkCanvas* canvas);

       private:
        /**
         * The paint for the connector lines
         */
        static SkPaint connectorPaint;

        /**
         * The paint for the connector lines that form a cycle
         */
        static SkPaint cycleConnectorPaint;
    };
}  // namespace gui::elements

#endif  // BLOCKS_MANAGER_RENDERER_H
