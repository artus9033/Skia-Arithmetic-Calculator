#include "SkiaGLFWQtWindowImpl.h"

namespace gui::window {
    // explicitly define the GLFWQtWindowImpl specialization for SkiaRendererImpl
    /**
     * @brief The specialization of the GLFWQtWindowImpl for the SkiaRendererImpl (Skia renderer)
     */
    template class GLFWQtWindowImpl<gui::renderer::SkiaRendererImpl, SkCanvas>;
}  // namespace gui::window
