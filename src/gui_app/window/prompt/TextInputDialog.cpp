#include "TextInputDialog.h"

namespace gui::window::prompt {
    std::optional<std::string> TextInputDialog::promptForTextInput(
        const std::string& title,
        const std::string& prompt,
        const std::string& defaultValue,
        business_logic::delegate::IWindowDelegate* windowDelegate) {
        const auto app = createTempQApplication();

        bool ok = false;
        const QString text = QInputDialog::getText(nullptr,
                                                   QString::fromStdString(title),
                                                   QString::fromStdString(prompt),
                                                   QLineEdit::Normal,
                                                   QString::fromStdString(defaultValue),
                                                   &ok);

        // restore focus to the window
        windowDelegate->focusWindow();

        if (ok && !text.isEmpty()) {
            return text.toStdString();
        } else {
            return std::nullopt;
        }

        return std::nullopt;
    }

    std::optional<FloatingPoint> TextInputDialog::promptForFloatingPointInput(
        const std::string& title,
        const std::string& prompt,
        const std::optional<FloatingPoint>& defaultValue,
        business_logic::delegate::IWindowDelegate* windowDelegate) {
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
