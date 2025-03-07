#include "TextInputDialog.h"

namespace gui::window::prompt {
    std::optional<std::string> TextInputDialog::promptForTextInput(
        const std::string title&,
        const std::string prompt&,
        const std::string& defaultValue,
        business_logic::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

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

    std::optional<FloatingPoint> TextInputDialog::promptForFloatingPointInput(
        const std::string title&,
        const std::string prompt&,
        const std::optional<FloatingPoint>& defaultValue,
        business_logic::delegate::IWindowDelegate* windowDelegate) {
        ensureQApplication();

        auto input = promptForTextInput(
            title, prompt, defaultValue ? (defaultValue.value()).str() : "", windowDelegate);

        if (!input.has_value()) {
            return std::nullopt;
        }

        auto strValue = input.value();
        // replace , with .
        std::ranges::replace(strValue, ',', '.');

        try {
            return FloatingPoint(strValue);
        } catch (const std::runtime_error&) {
            return std::nullopt;
        }
    }

}  // namespace gui::window::prompt
