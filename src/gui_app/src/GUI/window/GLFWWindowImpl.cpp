#include "GUI/window/GLFWWindowImpl.h"

#include "GUI/renderer/SkiaRendererImpl.h"

namespace gui::window {
    // explicitly define the static members of Window specialization for SkiaRendererImpl
    template <>
    bool GLFWWindowImpl<gui::renderer::SkiaRendererImpl>::initializedGLFW = false;

    template class GLFWWindowImpl<gui::renderer::SkiaRendererImpl>;

    // explicitly define the static member for the template to omit -Wundefined-var-template
    template <IsIRenderer RendererImpl>
    bool GLFWWindowImpl<RendererImpl>::initializedGLFW = false;
}  // namespace gui::window
