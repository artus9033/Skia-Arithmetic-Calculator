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

#include <algorithm>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <vector>

#include "FontManager.h"
#include "business_logic/BlocksManager.h"
#include "business_logic/components/UIText.h"
#include "business_logic/components/UITextsRow.h"
#include "business_logic/elements/blocks/BaseBlock.h"
#include "business_logic/geometry/Size2D.h"
#include "constants.h"
#include "delegate/UIRendererDelegate.h"
#include "logging/Loggable.h"
#include "renderer/colors.h"
#include "window/WindowBase.h"

// below: 20% margin between texts, 10% margin between rows
// for SkiaRendererImpl::renderCenteredTextsRows
constexpr float CENTERED_TEXT_ROWS_MARGIN_HORIZONTAL_NORM_PERCENT = 0.5F;
constexpr float CENTERED_TEXT_ROWS_MARGIN_VERTICAL_NORM_PERCENT = 0.35F;

namespace gui::renderer {
    // using an empty namespace to avoid polluting the outer scope of gui::renderer
    namespace {
        namespace colors = gui::renderer::colors;
        namespace components = business_logic::components;
    }  // namespace

    /**
     * @brief Class responsible for rendering the GUI using Skia and GLFW
     */
    class SkiaRendererImpl : public gui::renderer::delegate::UIRendererDelegate<SkCanvas>,
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
        SkiaRendererImpl(gui::window::WindowBase<SkCanvas>* window,
                         std::shared_ptr<business_logic::BlocksManager> blocksManager);

        /**
         * @brief Destructor
         */
        ~SkiaRendererImpl() override;

        // disable copy semantics
        SkiaRendererImpl(const SkiaRendererImpl&) = delete;
        SkiaRendererImpl& operator=(const SkiaRendererImpl&) = delete;

        // disable move semantics
        SkiaRendererImpl(SkiaRendererImpl&&) = delete;
        SkiaRendererImpl& operator=(SkiaRendererImpl&&) = delete;

        /**
         * @brief Renders the current frame
         */
        void render();

        /**
         * @brief Handles window resize events
         * @param window The window
         * @param xScale New window x scale
         * @param yScale New window y scale
         */
        void handleWindowResized(gui::window::WindowBase<SkCanvas>* window,
                                 float xScale,
                                 float yScale);

        /**
         * \copydoc gui::renderer::delegate::UIRendererDelegate::renderCenteredTextsRows
         */
        void renderCenteredTextsRows(SkCanvas* canvas,
                                     const business_logic::geometry::Size2D& size,
                                     const std::vector<components::UITextsRow>& rows) override;

       private:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<SkiaRendererImpl>::logger;

        /**
         * @brief The blocks manager
         */
        std::shared_ptr<business_logic::BlocksManager> blocksManager;

        /**
         * @brief The window
         */
        [[maybe_unused]] gui::window::WindowBase<SkCanvas>* window;

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
        gui::renderer::delegate::UIRendererDelegate<SkCanvas>* uiRendererDelegatePtr;
    };

}  // namespace gui::renderer

#endif  // SKIA_RENDERER_H
