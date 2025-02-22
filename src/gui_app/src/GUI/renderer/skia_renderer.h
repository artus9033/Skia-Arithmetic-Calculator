#ifndef SKIA_RENDERER_H
#define SKIA_RENDERER_H

#include <GLFW/glfw3.h>

#include <stdexcept>

#include "GUI/window/window.h"
#include "skia/include/core/SkSurface.h"
#include "skia/include/gpu/ganesh/GrDirectContext.h"
#include "skia/include/gpu/ganesh/SkSurfaceGanesh.h"
#include "skia/include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "skia/include/gpu/ganesh/gl/GrGLInterface.h"

class Window;

/**
 * @brief Class responsible for rendering the GUI using Skia and GLFW
 */
class SkiaRenderer {
   public:
    /**
     * @brief Constructs a new SkiaRenderer
     * @param window GLFW window handle
     */
    explicit SkiaRenderer(Window* window, int width, int height);

    /**
     * @brief Destructor
     */
    ~SkiaRenderer();

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
    [[maybe_unused]] Window* window;
    int width;
    int height;

    sk_sp<GrDirectContext> grContext;
    sk_sp<SkSurface> skSurface;

    /**
     * @brief Reinitializes the Skia surface using the current width and height
     */
    void reinitializeSurface();
};

#endif  // SKIA_RENDERER_H