#include "GuiMessageBox.h"

namespace gui::window::prompt {
    void GuiMessageBox::showWarning(const std::string& title,
                                    const std::string& message,
                                    business_logic::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

        QMessageBox::warning(
            nullptr, QString::fromStdString(title), QString::fromStdString(message));

        // restore focus to the window
        windowDelegate->focusWindow();
    }

    bool GuiMessageBox::promptConfirmation(
        const std::string& title,
        const std::string& message,
        business_logic::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

        bool const retVal =
            QMessageBox::question(nullptr,
                                  QString::fromStdString(title),
                                  QString::fromStdString(message)) == QMessageBox::Yes;

        // restore focus to the window
        windowDelegate->focusWindow();

        return retVal;
    }
}  // namespace gui::window::prompt
