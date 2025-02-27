#ifndef TEXT_INPUT_DIALOG_H
#define TEXT_INPUT_DIALOG_H

#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <optional>
#include <string>

#include "GUI/window/delegate/IWindowDelegate.h"

namespace gui::logic {
    class TextInputDialog {
       public:
        TextInputDialog() = delete;

        static std::optional<std::string> promptForTextInput(
            std::string title,
            std::string prompt,
            const std::string& defaultValue,
            gui::window::delegate::IWindowDelegate* windowDelegate);

        static std::optional<double> promptForDoubleInput(
            std::string title,
            std::string prompt,
            const std::optional<double>& defaultValue,
            gui::window::delegate::IWindowDelegate* windowDelegate);
    };
}  // namespace gui::logic

#endif  // TEXT_INPUT_DIALOG_H
