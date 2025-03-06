#ifndef TEXT_INPUT_DIALOG_H
#define TEXT_INPUT_DIALOG_H

#include <QInputDialog>
#include <QString>
#include <optional>
#include <string>

#include "delegate/IWindowDelegate.h"
#include "typenames.h"
#include "window/prompt/QAppHolderProto.h"

namespace gui::window::prompt {
    /**
     * @brief Utility class for prompting the user for input
     *
     * @see gui::window::prompt::QAppHolderProto
     */
    class TextInputDialog : public QAppHolderProto {
       public:
        TextInputDialog() = delete;

        static std::optional<std::string> promptForTextInput(
            std::string title,
            std::string prompt,
            const std::string& defaultValue,
            business_logic::delegate::IWindowDelegate* windowDelegate);

        static std::optional<FloatingPoint> promptForFloatingPointInput(
            std::string title,
            std::string prompt,
            const std::optional<FloatingPoint>& defaultValue,
            business_logic::delegate::IWindowDelegate* windowDelegate);
    };
}  // namespace gui::window::prompt

#endif  // TEXT_INPUT_DIALOG_H
