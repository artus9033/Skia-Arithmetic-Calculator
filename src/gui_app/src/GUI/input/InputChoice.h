#ifndef GUI_INPUT_INPUT_CHOICE_H
#define GUI_INPUT_INPUT_CHOICE_H

#include <string>

namespace gui::input {
    /**
     * @brief Input choice DTO
     */
    template <typename T>
    class InputChoice {
       public:
        /**
         * @brief The display name of the input choice
         */
        std::string displayName;

        /**
         * @brief The input value of the input choice
         */
        T value;
    };
}  // namespace gui::input

#endif  // GUI_INPUT_INPUT_CHOICE_H
