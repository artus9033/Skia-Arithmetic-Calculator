#include "TextInputDialog.h"

namespace gui::logic {
    std::optional<std::string> TextInputDialog::promptForTextInput(
        std::string title,
        std::string prompt,
        const std::string& defaultValue,
        gui::window::delegate::IWindowDelegate* windowDelegate) {
        QString defaultValueQstr = QString::fromStdString(defaultValue);

        bool ok;
        QString text = QInputDialog::getText(nullptr,
                                             QString::fromStdString(title),
                                             QString::fromStdString(prompt),
                                             QLineEdit::Normal,
                                             defaultValueQstr,
                                             &ok);

        if (ok && !text.isEmpty()) {
            return text.toStdString();
        } else {
            return std::nullopt;
        }

        // restore focus to the window
        windowDelegate->focusWindow();
    }

    std::optional<double> TextInputDialog::promptForDoubleInput(
        std::string title,
        std::string prompt,
        const std::optional<double>& defaultValue,
        gui::window::delegate::IWindowDelegate* windowDelegate) {
        auto input = promptForTextInput(title,
                                        prompt,
                                        defaultValue ? std::to_string(defaultValue.value()) : "",
                                        windowDelegate);

        if (!input.has_value()) {
            return std::nullopt;
        }

        try {
            return std::stod(input.value());
        } catch (const std::invalid_argument&) {
            return std::nullopt;
        }
    }

}  // namespace gui::logic
