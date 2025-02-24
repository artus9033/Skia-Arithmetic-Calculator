#ifndef GUI_ELEMENTS_IMPL_CONSTANT_BLOCK_H
#define GUI_ELEMENTS_IMPL_CONSTANT_BLOCK_H

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/elements/base/IDoubleClickable.h"
#include "GUI/logic/delegate/INewBlockChoiceDelegate.h"
#include "logging/Loggable.h"
#include "utils/misc.h"

namespace gui::elements::impl {
    /**
     * A constant block that holds a constant value (scalar)
     *
     * Can be double-clicked to edit the value and dragged around to change its position.
     */
    class ConstantBlock : public business_logic::Loggable<ConstantBlock>,
                          public gui::elements::base::BaseBlock,
                          public gui::elements::base::IDoubleClickable {
       public:
        /**
         * \copydoc BaseBlock::BaseBlock
         */
        ConstantBlock(int cx,
                      int cy,
                      gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate);

        void render(SkCanvas* canvas, bool isHovered) const override;

        /**
         * \copydoc IDoubleClickable::onDoubleClick
         */
        void onDoubleClick(int x, int y) override;

        std::string getSelfId() const override { return selfId; }

       protected:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<ConstantBlock>::logger;

        /**
         * The address of the block in memory, used for logging purposes
         */
        std::string selfId;

        /**
         * The width of the block
         */
        static constexpr int CONSTANT_BLOCK_WIDTH = 100;

        /**
         * The height of the block
         */
        static constexpr int CONSTANT_BLOCK_HEIGHT = 100;

        /**
         * The delegate that is notified when a new block is chosen to be added to the canvas
         */
        gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate;
    };
}  // namespace gui::elements::impl

#endif  // GUI_ELEMENTS_IMPL_CONSTANT_BLOCK_H
