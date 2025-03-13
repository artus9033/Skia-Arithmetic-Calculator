#include "UITextsRow.h"

namespace business_logic::components {
    UITextsRow::UITextsRow(const std::vector<UIText>& texts) {
        this->texts.reserve(texts.size());
        std::copy(texts.begin(), texts.end(), std::back_inserter(this->texts));
    }

    UITextsRow::UITextsRow(const UITextsRow& other) : texts(other.texts) {}

    UITextsRow& UITextsRow::operator=(const UITextsRow& other) {
        if (this != &other) {
            this->texts = other.texts;
        }

        return *this;
    }

    UITextsRow::UITextsRow(UITextsRow&& other) : texts(std::move(other.texts)) {}

    UITextsRow& UITextsRow::operator=(UITextsRow&& other) {
        if (this != &other) {
            this->texts = std::move(other.texts);
        }

        return *this;
    }
}  // namespace business_logic::components
