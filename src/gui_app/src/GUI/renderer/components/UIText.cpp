#include "UIText.h"

namespace gui::renderer::components {

    UIText::UIText(const std::string& text, const Variant& variant)
        : text(std::move(text)), variant(variant) {}

}  // namespace gui::renderer::components
