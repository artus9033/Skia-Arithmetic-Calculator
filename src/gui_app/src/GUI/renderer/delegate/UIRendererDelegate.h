#ifndef UIRenderDelegate_H
#define UIRenderDelegate_H

#include <skia/include/core/SkCanvas.h>

#include <string>
#include <vector>

#include "GUI/geometry/Size2D.h"
#include "GUI/renderer/components/UITextsRow.h"

namespace gui::renderer::delegate {
    /**
     * @brief The delegate for the UI renderer
     *
     * The delegate is used to render the UI elements.
     *
     * @see gui::renderer::IRenderer
     */
    class UIRendererDelegate {
       public:
        virtual ~UIRendererDelegate() = default;

        /**
         * @brief Renders the texts on the canvas around the center horizontally,
         * centered vertically
         * @param canvas SkCanvas to draw on
         * @param size The size of the canvas
         * @param rows The rows of texts to render
         */
        virtual void renderCenteredTextsRows(SkCanvas* canvas,
                                             const geometry::Size2D& size,
                                             const std::vector<components::UITextsRow>& rows) = 0;
    };
}  // namespace gui::renderer::delegate

#endif  // UIRenderDelegate_H
