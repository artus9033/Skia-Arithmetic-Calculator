#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMgr.h>
#include <skia/include/core/SkPaint.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkTypeface.h>

#include "colors.h"

#ifdef __APPLE__
#include <skia/include/ports/SkFontMgr_mac_ct.h>
#else
#include <skia/include/core/SkFontMgr.h>
#endif

#define MENU_HEADLINE_FONT_SIZE_BASE 17
#define MENU_CAPTION_FONT_SIZE_BASE 10
#define CAPTION_FONT_SIZE_BASE 7
#define MENU_CHOICE_FONT_SIZE_BASE 14

namespace gui::renderer {
    // using an empty namespace to avoid polluting the outer scope of gui::elements::base
    namespace {
        namespace colors = gui::renderer::colors;
    }

    class FontManager {
       public:
        /**
         * @brief The paint for the text font fill
         */
        static SkPaint textFontFillPaint;

        /**
         * @brief The paint for the text font stroke
         */
        static SkPaint textFontStrokePaint;

        /**
         * @brief The font for the menu choice font
         */
        static SkFont menuChoiceFont;

        /**
         * @brief The font for the menu headline font
         */
        static SkFont menuHeadlineFont;

        /**
         * @brief The font for the menu caption font
         */
        static SkFont menuCaptionFont;

        /**
         * @brief The font for the caption font
         */
        static SkFont captionFont;

        /**
         * @brief Recalculates the font sizes; must be called when the window is resized
         * by the `IRenderer` instance
         * @param xScale The scale factor for the x-axis
         */
        static void recalculateFontSizes(float xScale);

       private:
        /**
         * @brief The typeface for the text font
         */
        static sk_sp<SkTypeface> typeface;

        /**
         * @brief The font manager
         */
        static sk_sp<SkFontMgr> fontMgr;
    };
}  // namespace gui::renderer

#endif  // FONT_MANAGER_H
