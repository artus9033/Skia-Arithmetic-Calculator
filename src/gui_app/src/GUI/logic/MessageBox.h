#pragma once

#include <QApplication>
#include <QMessageBox>
#include <string>

#include "GUI/window/delegate/IWindowDelegate.h"

namespace gui::logic {

    class MessageBox {
       public:
        static void showWarning(const std::string& title,
                                const std::string& message,
                                gui::window::delegate::IWindowDelegate* windowDelegate);

       private:
        static void ensureQApplication();
    };

}  // namespace gui::logic
