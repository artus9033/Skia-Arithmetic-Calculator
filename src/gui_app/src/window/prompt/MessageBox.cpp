#include "MessageBox.h"

namespace gui::window::prompt {
    void MessageBox::showWarning(const std::string& title,
                                 const std::string& message,
                                 business_logic::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

        QMessageBox::warning(
            nullptr, QString::fromStdString(title), QString::fromStdString(message));

        // restore focus to the window
        windowDelegate->focusWindow();
    }

    bool MessageBox::promptConfirmation(const std::string& title,
                                        const std::string& message,
                                        business_logic::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

        bool retVal = QMessageBox::question(nullptr,
                                            QString::fromStdString(title),
                                            QString::fromStdString(message)) == QMessageBox::Yes;

        // restore focus to the window
        windowDelegate->focusWindow();

        return retVal;
    }
}  // namespace gui::window::prompt
