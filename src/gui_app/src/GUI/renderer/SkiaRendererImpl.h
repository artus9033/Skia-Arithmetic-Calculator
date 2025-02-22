#ifndef SKIA_RENDERER_H
#define SKIA_RENDERER_H

#include <GLFW/glfw3.h>

#include <stdexcept>

#include "GUI/renderer/IRenderer.h"
#include "GUI/window/IWindow.h"
#include "logging/Loggable.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/ganesh/GrDirectContext.h"
#include "skia/include/gpu/ganesh/SkSurfaceGanesh.h"
#include "skia/include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "skia/include/gpu/ganesh/gl/GrGLInterface.h"

namespace gui::renderer {
    /**
     * @brief Class responsible for rendering the GUI using Skia and GLFW
     */
    class SkiaRendererImpl : public IRenderer, protected business_logic::Loggable {
       public:
        /**
         * @brief Constructs a new SkiaRendererImpl
         * @param window GLFW window handle
         */
        explicit SkiaRendererImpl(gui::window::IWindow* window, int width, int height);

        /**
         * @brief Destructor
         */
        ~SkiaRendererImpl();

        /**
         * @brief Renders the current frame
         */
        void render();

        /**
         * @brief Handles window resize events
         * @param width New window width
         * @param height New window height
         */
        void handleWindowResized(int width, int height);

       private:
        [[maybe_unused]] gui::window::IWindow* window;
        int width;
        int height;

        sk_sp<GrDirectContext> grContext;
        sk_sp<SkSurface> skSurface;

        /**
         * @brief Reinitializes the Skia surface using the current width and height
         */
        void reinitializeSurface();
    };

}  // namespace gui::renderer

#endif  // SKIA_RENDERER_H
