#ifndef BUSINESS_LOGIC_INPUT_INPUT_CHOICE_H
#define BUSINESS_LOGIC_INPUT_INPUT_CHOICE_H

#include <string>

namespace business_logic::input {
    /**
     * @brief Input choice DTO
     */
    template <typename T>
    class InputChoice final {
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
}  // namespace business_logic::input

#endif  // BUSINESS_LOGIC_INPUT_INPUT_CHOICE_H
