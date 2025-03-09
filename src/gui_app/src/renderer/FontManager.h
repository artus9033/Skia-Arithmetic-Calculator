#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMgr.h>
#include <skia/include/core/SkPaint.h>
#include <skia/include/core/SkSurface.h>
#include <skia/include/core/SkTypeface.h>

#include "business_logic/components/UIText.h"
#include "colors.h"
#include "constants.h"

#ifdef APPLE
#include <skia/include/ports/SkFontMgr_mac_ct.h>
#elif defined(__linux__)
// linux - use fontconfig
#include "include/ports/SkFontMgr_fontconfig.h"
#include "include/ports/SkFontScanner_FreeType.h"
#else
// windows
#include "include/ports/SkTypeface_win.h"
#endif

/**
 * @brief The GUI renderer and renderer-supporting implementation module
 */
namespace gui::renderer {
    // using an empty namespace to avoid polluting the outer scope of business_logic::elements::base
    namespace {
        namespace colors = gui::renderer::colors;
    }

    /**
     * @brief Utility class that holds static font instances and handles scaling them on window
     * resizes
     */
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
         * @brief Recalculates the font sizes
         * @param xScale The scale factor for the x-axis
         */
        static void recalculateFontSizes(float xScale);

        /**
         * @brief Gets the font for the given variant
         * @param variant The variant of the label
         * @return The font for the given variant
         */
        static const SkFont& getFontForVariant(
            const business_logic::components::UIText::Variant& variant);

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
