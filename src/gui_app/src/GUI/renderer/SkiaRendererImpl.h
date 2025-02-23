#ifndef SKIA_RENDERER_H
#define SKIA_RENDERER_H

#include <GLFW/glfw3.h>

#include <stdexcept>

#include "GUI/renderer/IRenderer.h"
#include "GUI/window/IWindow.h"
#include "logging/Loggable.h"
#include "skia/include/core/SkCanvas.h"
#include "skia/include/core/SkColorSpace.h"
#include "skia/include/core/SkFont.h"
#include "skia/include/core/SkPaint.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/ganesh/GrBackendSurface.h"
#include "skia/include/gpu/ganesh/GrDirectContext.h"
#include "skia/include/gpu/ganesh/SkSurfaceGanesh.h"
#include "skia/include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "skia/include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "skia/include/gpu/ganesh/gl/GrGLInterface.h"

namespace gui::renderer {
    /**
     * @brief Class responsible for rendering the GUI using Skia and GLFW
     */
    class SkiaRendererImpl : public IRenderer,
                             protected business_logic::Loggable<SkiaRendererImpl> {
       public:
        /**
         * @brief Constructs a new SkiaRendererImpl
         * @param window GLFW window handle
         * @param winWidth Window width
         * @param winHeight Window height
         * @param fbWidth Framebuffer width
         * @param fbHeight Framebuffer height
         */
        explicit SkiaRendererImpl(
            gui::window::IWindow* window, int winWidth, int winHeight, int fbWidth, int fbHeight);

        /**
         * @brief Destructor
         */
        ~SkiaRendererImpl();

        void render() override;

        void handleWindowResized(int winWidth,
                                 int winHeight,
                                 int fbWidth,
                                 int fbHeight,
                                 double xScale,
                                 double yScale) override;

       private:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<SkiaRendererImpl>::logger;

        [[maybe_unused]] gui::window::IWindow* window;
        int winWidth;
        int winHeight;
        int fbWidth;
        int fbHeight;

        sk_sp<GrDirectContext> grContext;
        sk_sp<SkSurface> skSurface;

        /**
         * @brief Reinitializes the Skia surface using the current width and height
         */
        void reinitializeSurface();
    };

}  // namespace gui::renderer

#endif  // SKIA_RENDERER_H
