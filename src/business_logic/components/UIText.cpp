#include "UIText.h"

namespace business_logic::components {
    UIText::UIText(const std::string& text, const Variant& variant)
        : text(text), variant(variant) {}

    UIText& UIText::operator=(const UIText& other) {
        if (this != &other) {
            text = other.text;
            variant = other.variant;
        }

        return *this;
    }

    UIText::UIText(UIText&& other) noexcept : text(std::move(other.text)), variant(other.variant) {}

    UIText& UIText::operator=(UIText&& other) noexcept {
        if (this != &other) {
            text = std::move(other.text);
            variant = other.variant;
        }

        return *this;
    }
}  // namespace business_logic::components
