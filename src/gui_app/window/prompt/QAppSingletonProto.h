#ifndef QAPP_HOLDER_PROTO_H
#define QAPP_HOLDER_PROTO_H

#include <QApplication>

/**
 * @brief GUI user prompt utilities
 */
namespace gui::window::prompt {
    /**
     * @brief An internal helper class for ensuring a QApplication is created
     *
     * This class asserts that a QApplication is created, which is required for e.g. Qt dialogs.
     */
    class QAppSingletonProto {
       public:
        static void ensureQApplication();

        virtual ~QAppSingletonProto() = default;
    };
}  // namespace gui::window::prompt
#endif  // QAPP_HOLDER_PROTO_H
