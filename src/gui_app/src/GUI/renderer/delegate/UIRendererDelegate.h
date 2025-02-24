#ifndef UIRenderDelegate_H
#define UIRenderDelegate_H

#include <string>
#include <vector>

#include "GUI/geometry/Size2D.h"
#include "GUI/renderer/components/UITextsRow.h"
#include "skia/include/core/SkCanvas.h"

namespace gui::renderer::delegate {

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
