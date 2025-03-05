#ifndef SKIA_RENDERER_H
#define SKIA_RENDERER_H

#include <GLFW/glfw3.h>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkColorSpace.h>
#include <skia/include/core/SkPaint.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/gpu/ganesh/GrBackendSurface.h>
#include <skia/include/gpu/ganesh/GrDirectContext.h>
#include <skia/include/gpu/ganesh/SkSurfaceGanesh.h>
#include <skia/include/gpu/ganesh/gl/GrGLBackendSurface.h>
#include <skia/include/gpu/ganesh/gl/GrGLDirectContext.h>
#include <skia/include/gpu/ganesh/gl/GrGLInterface.h>

#include <numeric>
#include <optional>
#include <stdexcept>
#include <vector>

#include "FontManager.h"
#include "GUI/elements/base/BaseBlock.h"
#include "GUI/geometry/Size2D.h"
#include "GUI/logic/BlocksManager.h"
#include "GUI/renderer/IRenderer.h"
#include "GUI/renderer/colors.h"
#include "GUI/renderer/delegate/UIRendererDelegate.h"
#include "GUI/window/WindowBase.h"
#include "components/UIText.h"
#include "components/UITextsRow.h"
#include "constants.h"
#include "logging/Loggable.h"

// below: 20% margin between texts, 10% margin between rows
// for SkiaRendererImpl::renderCenteredTextsRows
#define CENTERED_TEXT_ROWS_MARGIN_HORIZONTAL_NORM_PERCENT 0.5f
#define CENTERED_TEXT_ROWS_MARGIN_VERTICAL_NORM_PERCENT 0.35f

namespace gui::renderer {
    // using an empty namespace to avoid polluting the outer scope of gui::renderer
    namespace {
        namespace colors = gui::renderer::colors;
    }

    /**
     * @brief Class responsible for rendering the GUI using Skia and GLFW
     */
    class SkiaRendererImpl : public IRenderer,
                             public delegate::UIRendererDelegate,
                             protected business_logic::Loggable<SkiaRendererImpl> {
       public:
        /**
         * @brief Constructs a new SkiaRendererImpl
         * @param window GLFW window handle
         * @param winWidth Window width
         * @param winHeight Window height
         * @param fbWidth Framebuffer width
         * @param fbHeight Framebuffer height
         * @param blocksManager Blocks manager
         */
        explicit SkiaRendererImpl(gui::window::WindowBase* window,
                                  int winWidth,
                                  int winHeight,
                                  int fbWidth,
                                  int fbHeight,
                                  std::shared_ptr<gui::logic::BlocksManager> blocksManager);

        /**
         * @brief Destructor
         */
        ~SkiaRendererImpl();

        /**
         * \copydoc IRenderer::render
         */
        void render() override;

        /**
         * \copydoc IRenderer::handleWindowResized
         */
        void handleWindowResized(gui::window::WindowBase* window,
                                 double xScale,
                                 double yScale) override;

        /**
         * \copydoc gui::renderer::delegate::UIRendererDelegate::renderCenteredTextsRows
         */
        void renderCenteredTextsRows(SkCanvas* canvas,
                                     const geometry::Size2D& size,
                                     const std::vector<components::UITextsRow>& rows) override;

       private:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<SkiaRendererImpl>::logger;

        /**
         * @brief The blocks manager
         */
        std::shared_ptr<gui::logic::BlocksManager> blocksManager;

        /**
         * @brief The window
         */
        [[maybe_unused]] gui::window::WindowBase* window;

        /**
         * @brief The Skia context
         */
        sk_sp<GrDirectContext> grContext;

        /**
         * @brief The Skia surface
         */
        sk_sp<SkSurface> skSurface;

        /**
         * @brief Reinitializes the Skia surface using the current width and height
         */
        void reinitializeSurface();

        /**
         * @brief The UI renderer delegate
         */
        delegate::UIRendererDelegate* uiRendererDelegatePtr;
    };

}  // namespace gui::renderer

#endif  // SKIA_RENDERER_H
