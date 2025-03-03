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

#ifdef __APPLE__
    sk_sp<SkFontMgr> FontManager::fontMgr = SkFontMgr_New_CoreText(nullptr);
#else
    sk_sp<SkFontMgr> FontManager::fontMgr = SkFontMgr_New_Default();
#endif

    sk_sp<SkTypeface> FontManager::typeface =
        fontMgr->legacyMakeTypeface(nullptr, SkFontStyle::Normal());

    SkFont FontManager::menuHeadlineFont = [] {
        SkFont font;
        font.setSize(MENU_HEADLINE_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);
        font.setEmbolden(true);

        return font;
    }();

    SkFont FontManager::menuCaptionFont = [] {
        SkFont font;
        font.setSize(MENU_CAPTION_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);

        return font;
    }();

    SkFont FontManager::captionFont = [] {
        SkFont font;
        font.setSize(MENU_CAPTION_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);

        return font;
    }();

    SkFont FontManager::menuChoiceFont = [] {
        SkFont font;
        font.setSize(MENU_CHOICE_FONT_SIZE_BASE);
        font.setTypeface(FontManager::typeface);

        return font;
    }();

    void FontManager::recalculateFontSizes(float xScale) {
        menuHeadlineFont.setSize(MENU_HEADLINE_FONT_SIZE_BASE * xScale);
        menuCaptionFont.setSize(MENU_CAPTION_FONT_SIZE_BASE * xScale);
        menuChoiceFont.setSize(MENU_CHOICE_FONT_SIZE_BASE * xScale);
        captionFont.setSize(CAPTION_FONT_SIZE_BASE * xScale);
    }

}  // namespace gui::renderer
