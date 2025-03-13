#include "SkiaGLFWWindowImpl.h"

namespace gui::window {
    // explicitly define the GLFWWindowImpl specialization for SkiaRendererImpl
    template class GLFWWindowImpl<gui::renderer::SkiaRendererImpl, SkCanvas>;
}  // namespace gui::window
