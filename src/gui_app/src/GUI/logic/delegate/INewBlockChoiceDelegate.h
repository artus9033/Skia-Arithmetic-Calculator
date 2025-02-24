#ifndef GUI_LOGIC_DELEGATE_INEW_BLOCK_CHOICE_DELEGATE_H
#define GUI_LOGIC_DELEGATE_INEW_BLOCK_CHOICE_DELEGATE_H

#include "GUI/elements/base/BlockType.h"

namespace gui::logic::delegate {
    /**
     * @brief Delegate that is notified when a new block is chosen to be added to the canvas
     */
    class INewBlockChoiceDelegate {
       public:
        virtual ~INewBlockChoiceDelegate() = default;

        /**
         * @brief Called when a new block is chosen
         * @param blockType The type of the block that was chosen
         */
        virtual void onNewBlockChoice(const gui::elements::base::BlockType& blockType) = 0;
    };
}  // namespace gui::logic::delegate

#endif  // GUI_LOGIC_DELEGATE_INEW_BLOCK_CHOICE_DELEGATE_H
