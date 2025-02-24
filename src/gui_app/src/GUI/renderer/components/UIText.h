#ifndef UITEXT_H
#define UITEXT_H

#include <string>

#include "skia/include/core/SkFont.h"
#include "skia/include/core/SkPaint.h"

namespace gui::renderer::components {

    class UIText {
       public:
        enum class Variant {
            Headline,
            Choice,
        };

        UIText(const std::string& text, const Variant& variant);

        const std::string& getText() const { return text; }
        const Variant& getVariant() const { return variant; }

       private:
        std::string text;
        Variant variant;
    };

}  // namespace gui::renderer::components
#endif  // UITEXT_H
