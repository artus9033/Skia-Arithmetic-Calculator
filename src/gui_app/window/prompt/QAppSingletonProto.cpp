#include "QAppSingletonProto.h"

namespace gui::window::prompt {
    std::unique_ptr<QApplication> QAppSingletonProto::createTempQApplication() {
        int fakeArgc = 0;
        char** fakeArgv = nullptr;

        return std::make_unique<QApplication>(fakeArgc, fakeArgv);
    }
}  // namespace gui::window::prompt
