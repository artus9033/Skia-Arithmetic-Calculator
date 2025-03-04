#ifndef GUI_ELEMENTS_IMPL_SUBTRACT_BLOCK_H
#define GUI_ELEMENTS_IMPL_SUBTRACT_BLOCK_H

#include "GUI/elements/base/BaseBlock.h"
#include "logging/Loggable.h"
#include "typenames.h"
#include "utils/misc.h"

namespace gui::elements::impl {
    class SubtractBlock : public business_logic::Loggable<SubtractBlock>,
                          public gui::elements::base::BaseBlock {
       public:
        SubtractBlock(
            int cx,
            int cy,
            gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            gui::window::delegate::IWindowDelegate* windowDelegate);

        void render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) override;

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override;

       protected:
        using business_logic::Loggable<SubtractBlock>::logger;

        std::string selfId;

        static constexpr int SUBTRACT_BLOCK_WIDTH = 120;
        static constexpr int SUBTRACT_BLOCK_HEIGHT = 120;

        const std::vector<gui::elements::base::Port>& getInputPorts() const override {
            return inputPorts;
        }

        const std::vector<gui::elements::base::Port>& getOutputPorts() const override {
            return outputPorts;
        }

        gui::elements::base::BlockType getBlockType() const override {
            return gui::elements::base::BlockType::Subtract;
        }

       protected:
        static const std::vector<gui::elements::base::Port> inputPorts;
        static const std::vector<gui::elements::base::Port> outputPorts;
    };
}  // namespace gui::elements::impl

#endif  // GUI_ELEMENTS_IMPL_SUBTRACT_BLOCK_H
