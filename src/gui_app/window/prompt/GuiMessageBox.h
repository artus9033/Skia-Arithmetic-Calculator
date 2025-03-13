#ifndef GUI_WINDOW_PROMPT_MESSAGE_BOX_H
#define GUI_WINDOW_PROMPT_MESSAGE_BOX_H

#include <QMessageBox>
#include <string>

#include "delegate/IWindowDelegate.h"
#include "window/prompt/QAppSingletonProto.h"
namespace gui::window::prompt {
    /**
     * @brief Utility class for displaying messages to the user
     *
     * @see gui::window::prompt::QAppSingletonProto
     */
    class GuiMessageBox final : public QAppSingletonProto {
       public:
        GuiMessageBox() = delete;

        static void showWarning(const std::string& title,
                                const std::string& message,
                                business_logic::delegate::IWindowDelegate* windowDelegate);

        static bool promptConfirmation(const std::string& title,
                                       const std::string& message,
                                       business_logic::delegate::IWindowDelegate* windowDelegate);
    };
}  // namespace gui::window::prompt

#endif  // GUI_WINDOW_PROMPT_MESSAGE_BOX_H
