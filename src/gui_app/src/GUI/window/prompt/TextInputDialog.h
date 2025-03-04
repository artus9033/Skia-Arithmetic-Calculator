#ifndef TEXT_INPUT_DIALOG_H
#define TEXT_INPUT_DIALOG_H

#include <QInputDialog>
#include <QString>
#include <optional>
#include <string>

#include "GUI/window/delegate/IWindowDelegate.h"
#include "GUI/window/prompt/QAppHolderProto.h"
#include "typenames.h"

namespace gui::window::prompt {
    class TextInputDialog : public QAppHolderProto {
       public:
        TextInputDialog() = delete;

        static std::optional<std::string> promptForTextInput(
            std::string title,
            std::string prompt,
            const std::string& defaultValue,
            gui::window::delegate::IWindowDelegate* windowDelegate);

        static std::optional<FloatingPoint> promptForFloatingPointInput(
            std::string title,
            std::string prompt,
            const std::optional<FloatingPoint>& defaultValue,
            gui::window::delegate::IWindowDelegate* windowDelegate);
    };
}  // namespace gui::window::prompt

#endif  // TEXT_INPUT_DIALOG_H
