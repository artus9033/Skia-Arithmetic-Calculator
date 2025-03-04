#include "MessageBox.h"

namespace gui::window::prompt {
    void MessageBox::showWarning(const std::string& title,
                                 const std::string& message,
                                 gui::window::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

        QMessageBox::warning(
            nullptr, QString::fromStdString(title), QString::fromStdString(message));

        // restore focus to the window
        windowDelegate->focusWindow();
    }
}  // namespace gui::window::prompt
