#ifndef UITEXT_H
#define UITEXT_H

#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkPaint.h>

#include <string>

namespace gui::renderer::components {
    /**
     * @brief The UI renderable text primitive
     */
    class UIText {
       public:
        /**
         * @brief The visual display variant of the text
         */
        enum class Variant { Headline, MenuCaption, Choice, Caption };

        /**
         * @brief Constructor
         * @param text The text to display
         * @param variant The visual display variant of the text
         */
        UIText(const std::string& text, const Variant& variant);

        /**
         * @brief Gets the text
         * @return The text
         */
        const std::string& getText() const { return text; }

        /**
         * @brief Gets the font for the given variant
         * @param variant The variant of the label
         * @return The font for the given variant
         */
        static const SkFont& getFontForVariant(const Variant& variant);

       private:
        /**
         * @brief The text to display
         */
        std::string text;

        /**
         * @brief The visual display variant of the text
         */
        Variant variant;
    };

}  // namespace gui::renderer::components
#endif  // UITEXT_H
