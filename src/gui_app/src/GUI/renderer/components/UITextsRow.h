#ifndef UITEXTSROW_H
#define UITEXTSROW_H

#include <vector>

#include "UIText.h"

namespace gui::renderer::components {
    /**
     * @brief The UI renderable row of `UIText`s primitive
     *
     * @see gui::renderer::components::UIText
     */
    class UITextsRow {
       public:
        /**
         * @brief Constructor
         * @param texts The texts to display in the row
         */
        UITextsRow(const std::vector<UIText>& texts);

        /**
         * @brief Gets the texts in this row
         * @return The texts in this row
         */
        const std::vector<UIText>& getUiTexts() const { return texts; }

       private:
        /**
         * @brief The texts in this row
         */
        std::vector<UIText> texts;
    };

}  // namespace gui::renderer::components

#endif  // UITEXTSROW_H
