#include "UITextsRow.h"

namespace gui::renderer::components {

    UITextsRow::UITextsRow(const std::vector<UIText>& texts) {
        this->texts.reserve(texts.size());
        std::copy(texts.begin(), texts.end(), std::back_inserter(this->texts));
    }

}  // namespace gui::renderer::components
