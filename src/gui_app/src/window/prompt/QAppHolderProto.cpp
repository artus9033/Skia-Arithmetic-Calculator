#include "QAppHolderProto.h"

namespace gui::window::prompt {
    void QAppHolderProto::ensureQApplication() {
        static int argc = 1;
        static char* argv[] = {const_cast<char*>("")};
        static QApplication app(argc, argv);
    }
}  // namespace gui::window::prompt
