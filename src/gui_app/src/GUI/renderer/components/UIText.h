#ifndef UITEXT_H
#define UITEXT_H

#include <string>

#include "skia/include/core/SkFont.h"
#include "skia/include/core/SkPaint.h"

namespace gui::renderer::components {

    class UIText {
       public:
        enum class Variant { Headline, MenuCaption, Choice, Caption };

        UIText(const std::string& text, const Variant& variant);

        const std::string& getText() const { return text; }
        const Variant& getVariant() const { return variant; }

        /**
         * @brief Gets the font for the given variant
         * @param variant The variant of the label
         * @return The font for the given variant
         */
        static const SkFont& getFontForVariant(const Variant& variant);

       private:
        std::string text;
        Variant variant;
    };

}  // namespace gui::renderer::components
#endif  // UITEXT_H
