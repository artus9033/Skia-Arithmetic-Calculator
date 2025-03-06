#ifndef BUSINESS_LOGIC_INPUT_INPUT_CHOICE_INTERACTION_H
#define BUSINESS_LOGIC_INPUT_INPUT_CHOICE_INTERACTION_H

#include <functional>
#include <vector>

#include "InputChoice.h"

namespace business_logic::input {
    /**
     * @brief Input choice interaction DTO
     */
    template <typename T>
    struct InputChoiceInteraction {
       public:
        /**
         * @brief The input choices
         */
        std::vector<InputChoice<T>> choices;

        /**
         * @brief The callback to call when an input choice is made
         */
        std::function<void(const T&)> callback;
    };
}  // namespace business_logic::input

#endif  // BUSINESS_LOGIC_INPUT_INPUT_CHOICE_INTERACTION_H
