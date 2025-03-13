#ifndef GUI_RENDERER_DELEGATE_UI_RENDERER_DELEGATE_H
#define GUI_RENDERER_DELEGATE_UI_RENDERER_DELEGATE_H

#include <string>
#include <vector>

#include "business_logic/components/UITextsRow.h"
#include "business_logic/geometry/Size2D.h"

/**
 * @brief Renderer delegates for re-use by other components to be able to anonymously invoke
 * functionalities of the main renderer implementation
 */
namespace gui::renderer::delegate {
    // using an empty namespace to avoid polluting the outer scope of business_logic
    namespace {
        namespace components = business_logic::components;
    }  // namespace

    /**
     * @brief The delegate for the UI renderer
     *
     * The delegate is used to render the UI elements.
     */
    template <typename Canvas>
    class UIRendererDelegate {
       public:
        virtual ~UIRendererDelegate() = default;

        /**
         * @brief Renders the texts on the canvas around the center horizontally,
         * centered vertically
         * @param canvas Canvas to draw on
         * @param size The size of the canvas
         * @param rows The rows of texts to render
         */
        virtual void renderCenteredTextsRows(Canvas* canvas,
                                             const business_logic::geometry::Size2D& size,
                                             const std::vector<components::UITextsRow>& rows) = 0;
    };
}  // namespace gui::renderer::delegate

#endif  // GUI_RENDERER_DELEGATE_UI_RENDERER_DELEGATE_H
