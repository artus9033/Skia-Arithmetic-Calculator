#include "UIText.h"

#include "GUI/renderer/FontManager.h"

namespace gui::renderer::components {

    UIText::UIText(const std::string& text, const Variant& variant)
        : text(std::move(text)), variant(variant) {}

    const SkFont& UIText::getFontForVariant(const Variant& variant) {
        switch (variant) {
            case Variant::Headline:
                return gui::renderer::FontManager::menuHeadlineFont;

            case Variant::MenuCaption:
                return gui::renderer::FontManager::menuCaptionFont;

            case Variant::Caption:
                return gui::renderer::FontManager::captionFont;

            case Variant::Choice:
                return gui::renderer::FontManager::menuChoiceFont;
        }
    }
}  // namespace gui::renderer::components
