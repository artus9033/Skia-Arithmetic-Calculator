#ifndef UITEXTSROW_H
#define UITEXTSROW_H

#include <iterator>
#include <vector>

#include "UIText.h"

namespace business_logic::components {
    /**
     * @brief The UI renderable row of `UIText`s primitive
     *
     * @see business_logic::components::UIText
     */
    class UITextsRow {
       public:
        /**
         * @brief Constructor
         * @param texts The texts to display in the row
         */
        explicit UITextsRow(const std::vector<UIText>& texts);

        virtual ~UITextsRow() = default;

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

}  // namespace business_logic::components

#endif  // UITEXTSROW_H
