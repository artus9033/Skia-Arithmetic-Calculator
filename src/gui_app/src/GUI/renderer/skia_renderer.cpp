#include "skia_renderer.h"

SkiaRenderer::SkiaRenderer(Window* window, int width, int height)
    : window(window), width(width), height(height) {
    if (!window) {
        throw std::runtime_error("Invalid window handle provided to SkiaRenderer");
    }

    reinitializeSurface();

    if (skSurface == nullptr) {
        throw std::runtime_error(
            "Failed to create Skia surface (SkSurfaces::RenderTarget returned null)");
    }

    // m_surface = SkSurfaces::WrapBackendRenderTarget(m_context.get(), renderTarget,
    //                                                 kBottomLeft_GrSurfaceOrigin,
    //                                                 kRGBA_8888_SkColorType, nullptr, nullptr);

    // if (m_surface == nullptr) {
    //     throw std::runtime_error("Failed to create Skia surface");
    // }
    // int winWidth, winHeight;
    // glfwGetWindowSize(window, &winWidth, &winHeight);

    // float xScale = static_cast<float>(fbWidth) / winWidth;
    // float yScale = static_cast<float>(fbHeight) / winHeight;
    // getCanvas()->scale(xScale, yScale);
}

void SkiaRenderer::reinitializeSurface() {
    sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
    sk_sp<GrDirectContext> context = GrDirectContexts::MakeGL(nullptr);
    SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
    skSurface =
        sk_sp<SkSurface>(SkSurfaces::RenderTarget(context.get(), skgpu::Budgeted::kNo, info));
}

SkiaRenderer::~SkiaRenderer() {
    skSurface.reset();
    grContext.reset();
}

void SkiaRenderer::render() {}

void SkiaRenderer::handleWindowResized(int width, int height) {
    this->width = width;
    this->height = height;

    reinitializeSurface();
}
