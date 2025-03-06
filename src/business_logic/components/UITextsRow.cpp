#include "UITextsRow.h"

namespace business_logic::components {

    UITextsRow::UITextsRow(const std::vector<UIText>& texts) {
        this->texts.reserve(texts.size());
        std::copy(texts.begin(), texts.end(), std::back_inserter(this->texts));
    }

}  // namespace business_logic::components
