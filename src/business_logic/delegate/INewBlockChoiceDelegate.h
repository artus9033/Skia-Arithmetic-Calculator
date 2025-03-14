#ifndef BUSINESS_LOGIC_LOGIC_DELEGATE_INEW_BLOCK_CHOICE_DELEGATE_H
#define BUSINESS_LOGIC_LOGIC_DELEGATE_INEW_BLOCK_CHOICE_DELEGATE_H

#include "elements/blocks/BlockType.h"

namespace business_logic::delegate {
    /**
     * @brief Delegate that is notified when a new block is chosen to be added to the canvas
     */
    class
        INewBlockChoiceDelegate {  // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
       public:
        virtual ~INewBlockChoiceDelegate() = default;

        /**
         * @brief Called when a new block is chosen
         * @param blockType The type of the block that was chosen
         */
        virtual void onNewBlockChoice(
            const business_logic::elements::blocks::BlockType& blockType) = 0;
    };
}  // namespace business_logic::delegate

#endif  // BUSINESS_LOGIC_LOGIC_DELEGATE_INEW_BLOCK_CHOICE_DELEGATE_H
