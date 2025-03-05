#ifndef GUI_ELEMENTS_IMPL_MULTIPLY_BLOCK_H
#define GUI_ELEMENTS_IMPL_MULTIPLY_BLOCK_H

#include "GUI/elements/base/BaseBlock.h"
#include "logging/Loggable.h"
#include "typenames.h"
#include "utils/misc.h"

namespace gui::elements::impl {
    /**
     * @brief A block that multiplies two input values together and outputs their product
     */
    class MultiplyBlock : public business_logic::Loggable<MultiplyBlock>,
                          public gui::elements::base::BaseBlock {
       public:
        MultiplyBlock(
            int cx,
            int cy,
            gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            gui::window::delegate::IWindowDelegate* windowDelegate);

        void render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) override;

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override;

       protected:
        using business_logic::Loggable<MultiplyBlock>::logger;

        std::string selfId;

        static constexpr int MULTIPLY_BLOCK_WIDTH = 120;
        static constexpr int MULTIPLY_BLOCK_HEIGHT = 120;

        const std::vector<gui::elements::base::Port>& getInputPorts() const override {
            return inputPorts;
        }

        const std::vector<gui::elements::base::Port>& getOutputPorts() const override {
            return outputPorts;
        }

        gui::elements::base::BlockType getBlockType() const override {
            return gui::elements::base::BlockType::Multiply;
        }

       protected:
        static const std::vector<gui::elements::base::Port> inputPorts;
        static const std::vector<gui::elements::base::Port> outputPorts;
    };
}  // namespace gui::elements::impl

#endif  // GUI_ELEMENTS_IMPL_MULTIPLY_BLOCK_H
