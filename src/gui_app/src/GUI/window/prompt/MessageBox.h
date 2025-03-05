#pragma once

#include <QMessageBox>
#include <string>

#include "GUI/window/delegate/IWindowDelegate.h"
#include "GUI/window/prompt/QAppHolderProto.h"
namespace gui::window::prompt {
    /**
     * @brief Utility class for displaying messages to the user
     *
     * @see gui::window::prompt::QAppHolderProto
     */
    class MessageBox : public QAppHolderProto {
       public:
        static void showWarning(const std::string& title,
                                const std::string& message,
                                gui::window::delegate::IWindowDelegate* windowDelegate);

        static bool promptConfirmation(const std::string& title,
                                       const std::string& message,
                                       gui::window::delegate::IWindowDelegate* windowDelegate);
    };
}  // namespace gui::window::prompt
