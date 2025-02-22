#include "GUI/window/GLFWWindowImpl.h"

#include "GUI/renderer/SkiaRendererImpl.h"

namespace gui::window {
    // explicitly define the static members of Window specialization for SkiaRendererImpl
    template <>
    bool GLFWWindowImpl<gui::renderer::SkiaRendererImpl>::initializedGLFW = false;

    template class GLFWWindowImpl<gui::renderer::SkiaRendererImpl>;
}  // namespace gui::window
