#include "SkiaRendererImpl.h"

namespace gui::renderer {
    SkiaRendererImpl::SkiaRendererImpl(gui::window::IWindow* window, int width, int height)
        : IRenderer(window, width, height),
          Loggable("SkiaRendererImpl"),
          window(window),
          width(width),
          height(height) {
        if (!window) {
            throw std::runtime_error("Invalid window handle provided to SkiaRendererImpl");
        }

        reinitializeSurface();

        if (skSurface == nullptr) {
            throw std::runtime_error(
                "Failed to create Skia surface (SkSurfaces::RenderTarget returned null)");
        }
    }

    void SkiaRendererImpl::reinitializeSurface() {
        logger->info("Reinitializing Skia surface with width {} and height {}", width, height);

        sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
        sk_sp<GrDirectContext> context = GrDirectContexts::MakeGL(interface);
        SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
        skSurface =
            sk_sp<SkSurface>(SkSurfaces::RenderTarget(context.get(), skgpu::Budgeted::kNo, info));
    }

    SkiaRendererImpl::~SkiaRendererImpl() {
        skSurface.reset();
        grContext.reset();
    }

    void SkiaRendererImpl::render() {}

    void SkiaRendererImpl::handleWindowResized(int width, int height) {
        this->width = width;
        this->height = height;

        reinitializeSurface();
    }
}  // namespace gui::renderer
