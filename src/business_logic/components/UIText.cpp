#include "UIText.h"

namespace business_logic::components {
    UIText::UIText(const std::string& text, const Variant& variant)
        : text(std::move(text)), variant(variant) {}
}  // namespace business_logic::components
