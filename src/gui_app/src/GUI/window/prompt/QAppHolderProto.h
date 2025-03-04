#ifndef QAPP_HOLDER_PROTO_H
#define QAPP_HOLDER_PROTO_H

#include <QApplication>

namespace gui::window::prompt {
    class QAppHolderProto {
       public:
        static void ensureQApplication();
    };
}  // namespace gui::window::prompt
#endif  // QAPP_HOLDER_PROTO_H
