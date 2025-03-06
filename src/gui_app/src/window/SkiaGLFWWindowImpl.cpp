#include "SkiaGLFWWindowImpl.h"

namespace gui::window {
    // explicitly define the static members of Window specialization for SkiaRendererImpl
    template <>
    bool GLFWWindowImpl<gui::renderer::SkiaRendererImpl, SkCanvas>::initializedGLFW = false;

    template class GLFWWindowImpl<gui::renderer::SkiaRendererImpl, SkCanvas>;
}  // namespace gui::window
