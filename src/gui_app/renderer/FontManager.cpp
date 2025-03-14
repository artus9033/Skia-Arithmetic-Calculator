#include "FontManager.h"

namespace gui::renderer {
    SkPaint FontManager::textFontStrokePaint = [] {
        SkPaint paint;

        // black stroke
        paint.setColor(colors::TEXT_OUTLINE_COLOR);
        paint.setStyle(SkPaint::kStroke_Style);
        paint.setStrokeWidth(4);
        paint.setAntiAlias(true);

        return paint;
    }();

    SkPaint FontManager::textFontFillPaint = [] {
        SkPaint paint;

        // white fill
        paint.setColor(colors::TEXT_COLOR);
        paint.setStyle(SkPaint::kFill_Style);
        paint.setAntiAlias(true);

        return paint;
    }();

#if __APPLE__
    sk_sp<SkFontMgr> FontManager::fontMgr = SkFontMgr_New_CoreText(nullptr);
#elif WIN32
    sk_sp<SkFontMgr> FontManager::fontMgr = SkFontMgr_New_DirectWrite();
#else
    // linux - use fontconfig
    sk_sp<SkFontMgr> FontManager::fontMgr =
        SkFontMgr_New_FontConfig(nullptr, SkFontScanner_Make_FreeType());
#endif

    sk_sp<SkTypeface> FontManager::typeface =
        fontMgr->legacyMakeTypeface(nullptr, SkFontStyle::Normal());

    SkFont FontManager::menuHeadlineFont = [] {
        SkFont font;
        font.setSubpixel(true);
        font.setSize(gui::constants::MENU_HEADLINE_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);
        font.setEmbolden(true);

        return font;
    }();

    SkFont FontManager::menuCaptionFont = [] {
        SkFont font;
        font.setSubpixel(true);
        font.setSize(gui::constants::MENU_CAPTION_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);

        return font;
    }();

    SkFont FontManager::captionFont = [] {
        SkFont font;
        font.setSubpixel(true);
        font.setSize(gui::constants::CAPTION_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);

        return font;
    }();

    SkFont FontManager::menuChoiceFont = [] {
        SkFont font;
        font.setSubpixel(true);
        font.setSize(gui::constants::MENU_CHOICE_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);

        return font;
    }();

    void FontManager::recalculateFontSizes(float aspectX,
                                           float aspectY,
                                           float xScale,
                                           float yScale) {
        const float scaling = std::min(xScale, yScale) * std::min(aspectX, aspectY);

        menuHeadlineFont.setSize(gui::constants::MENU_HEADLINE_FONT_SIZE_BASE * scaling);
        menuCaptionFont.setSize(gui::constants::MENU_CAPTION_FONT_SIZE_BASE * scaling);
        menuChoiceFont.setSize(gui::constants::MENU_CHOICE_FONT_SIZE_BASE * scaling);
        captionFont.setSize(gui::constants::CAPTION_FONT_SIZE_BASE * scaling);
    }

    const SkFont& FontManager::getFontForVariant(
        const business_logic::components::UIText::Variant& variant) {
        switch (variant) {
            case business_logic::components::UIText::Variant::Headline:
                return gui::renderer::FontManager::menuHeadlineFont;

            case business_logic::components::UIText::Variant::MenuCaption:
                return gui::renderer::FontManager::menuCaptionFont;

            default:
            case business_logic::components::UIText::Variant::Caption:
                return gui::renderer::FontManager::captionFont;

            case business_logic::components::UIText::Variant::Choice:
                return gui::renderer::FontManager::menuChoiceFont;
        }
    }
}  // namespace gui::renderer
