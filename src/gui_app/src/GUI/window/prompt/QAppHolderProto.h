#ifndef QAPP_HOLDER_PROTO_H
#define QAPP_HOLDER_PROTO_H

#include <QApplication>

namespace gui::window::prompt {
    /**
     * @brief An internal helper class for ensuring a QApplication is created
     *
     * This class ensures that a QApplication is created, which is required for e.g. Qt dialogs.
     */
    class QAppHolderProto {
       public:
        static void ensureQApplication();
    };
}  // namespace gui::window::prompt
#endif  // QAPP_HOLDER_PROTO_H
