#include "QAppHolderProto.h"

namespace gui::window::prompt {
    void QAppHolderProto::ensureQApplication() {
        if (QApplication::instance() == nullptr) {
            throw std::runtime_error("QApplication instance has not been set!");
        }
    }
}  // namespace gui::window::prompt
