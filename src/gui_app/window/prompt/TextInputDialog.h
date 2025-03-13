#ifndef TEXT_INPUT_DIALOG_H
#define TEXT_INPUT_DIALOG_H

#include <QInputDialog>
#include <QString>
#include <algorithm>
#include <optional>
#include <string>

#include "delegate/IWindowDelegate.h"
#include "typenames.h"
#include "window/prompt/QAppSingletonProto.h"

namespace gui::window::prompt {
    /**
     * @brief Utility class for prompting the user for input
     *
     * @see gui::window::prompt::QAppSingletonProto
     */
    class TextInputDialog final : public QAppSingletonProto {
       public:
        TextInputDialog() = delete;

        static std::optional<std::string> promptForTextInput(
            const std::string& title,
            const std::string& prompt,
            const std::string& defaultValue,
            business_logic::delegate::IWindowDelegate* windowDelegate);

        static std::optional<FloatingPoint> promptForFloatingPointInput(
            const std::string& title,
            const std::string& prompt,
            const std::optional<FloatingPoint>& defaultValue,
            business_logic::delegate::IWindowDelegate* windowDelegate);
    };
}  // namespace gui::window::prompt

#endif  // TEXT_INPUT_DIALOG_H
