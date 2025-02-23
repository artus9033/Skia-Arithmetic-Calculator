#include "SkiaRendererImpl.h"

namespace gui::renderer {
    SkiaRendererImpl::SkiaRendererImpl(gui::window::WindowBase* window,
                                       int winWidth,
                                       int winHeight,
                                       int fbWidth,
                                       int fbHeight,
                                       std::shared_ptr<gui::logic::BlocksManager> blocksManager)
        : IRenderer(window, winWidth, winHeight, fbWidth, fbHeight),
          business_logic::Loggable<SkiaRendererImpl>(),
          blocksManager(blocksManager),
          window(window),
          winWidth(winWidth),
          winHeight(winHeight),
          fbWidth(fbWidth),
          fbHeight(fbHeight) {
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
        logger->info(
            "Reinitializing Skia surface with width {} and height {}", winWidth, winHeight);

        sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
        if (!interface) {
            throw std::runtime_error("Failed to create GL interface");
        }

        grContext = GrDirectContexts::MakeGL(interface);

        GrGLFramebufferInfo framebufferInfo;
        framebufferInfo.fFBOID = 0;
        framebufferInfo.fFormat = GL_RGBA8;

        GrBackendRenderTarget renderTarget =
            GrBackendRenderTargets::MakeGL(fbWidth, fbHeight, 0, 8, framebufferInfo);

        skSurface = SkSurfaces::WrapBackendRenderTarget(grContext.get(),
                                                        renderTarget,
                                                        kBottomLeft_GrSurfaceOrigin,
                                                        kRGBA_8888_SkColorType,
                                                        nullptr,
                                                        nullptr);
    }

    SkiaRendererImpl::~SkiaRendererImpl() {
        skSurface.reset();
        grContext.reset();
    }

    void SkiaRendererImpl::render() {
        SkCanvas* canvas = skSurface->getCanvas();
        canvas->clear(constants::WINDOW_BACKGROUND_COLOR);

        blocksManager->render(canvas);

        grContext->flush();
    }

    void SkiaRendererImpl::handleWindowResized(
        int winWidth, int winHeight, int fbWidth, int fbHeight, double xScale, double yScale) {
        this->winWidth = winWidth;
        this->winHeight = winHeight;
        this->fbWidth = fbWidth;
        this->fbHeight = fbHeight;

        reinitializeSurface();

        SkCanvas* canvas = skSurface->getCanvas();

        canvas->scale(xScale, yScale);
    }
}  // namespace gui::renderer
