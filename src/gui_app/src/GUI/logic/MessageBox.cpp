#include "MessageBox.h"

namespace gui::logic {

    void MessageBox::ensureQApplication() {
        if (!qApp) {
            static int argc = 1;
            static char* argv[] = {const_cast<char*>("")};
            static QApplication app(argc, argv);
        }
    }

    void MessageBox::showWarning(const std::string& title,
                                 const std::string& message,
                                 gui::window::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();
        QMessageBox::warning(
            nullptr, QString::fromStdString(title), QString::fromStdString(message));

        // restore focus to the window
        windowDelegate->focusWindow();
    }

}  // namespace gui::logic
