#ifndef QAPP_HOLDER_PROTO_H
#define QAPP_HOLDER_PROTO_H

#include <QApplication>
#include <memory>

/**
 * @brief GUI user prompt utilities
 */
namespace gui::window::prompt {
    /**
     * @brief An internal helper class for handling QApplication
     */
    class
        QAppSingletonProto {  // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
       public:
        /**
         * @brief Create a temporary QApplication instance
         *
         * @return The temporary QApplication
         */
        static std::unique_ptr<QApplication> createTempQApplication();

        virtual ~QAppSingletonProto() = default;
    };
}  // namespace gui::window::prompt
#endif  // QAPP_HOLDER_PROTO_H
