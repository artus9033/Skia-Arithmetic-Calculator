#ifndef GUI_ELEMENTS_BASE_PORT_H
#define GUI_ELEMENTS_BASE_PORT_H

#include <string>

namespace gui::elements::base {
    /**
     * @brief Represents a port on a block
     */
    struct Port {
       public:
        /**
         * @brief The name/label of the port
         */
        std::string name;

        /**
         * @brief The type of the port
         */
        enum class Type { INPUT, OUTPUT };

        /**
         * @brief The type of the port
         */
        Type type;
    };
}  // namespace gui::elements::base

#endif  // GUI_ELEMENTS_BASE_PORT_H
