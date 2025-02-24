#ifndef UITEXTSROW_H
#define UITEXTSROW_H

#include <vector>

#include "UIText.h"

namespace gui::renderer::components {

    class UITextsRow {
       public:
        UITextsRow(const std::vector<UIText>& texts);

        const std::vector<UIText>& getUiTexts() const { return texts; }

       private:
        std::vector<UIText> texts;
    };

}  // namespace gui::renderer::components

#endif  // UITEXTSROW_H
