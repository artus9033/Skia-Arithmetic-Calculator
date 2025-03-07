#ifndef BUSINESS_LOGIC_ELEMENTS_IMPL_SUBTRACT_BLOCK_H
#define BUSINESS_LOGIC_ELEMENTS_IMPL_SUBTRACT_BLOCK_H

#include "elements/blocks/BaseBlock.h"
#include "logging/Loggable.h"
#include "typenames.h"
#include "utils/misc.h"

namespace business_logic::elements::blocks {
    /**
     * @brief A block that subtracts the second input value from the first input value
     */

    class SubtractBlock : public business_logic::Loggable<SubtractBlock>,
                          public business_logic::elements::blocks::BaseBlock {
       public:
        SubtractBlock(
            int cx,
            int cy,
            business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            business_logic::delegate::IWindowDelegate* windowDelegate);

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override;

       protected:
        using business_logic::Loggable<SubtractBlock>::logger;

        std::string selfId;

        static constexpr int SUBTRACT_BLOCK_WIDTH = 120;
        static constexpr int SUBTRACT_BLOCK_HEIGHT = 120;

        const std::vector<business_logic::elements::structures::Port>& getInputPorts()
            const override {
            return inputPorts;
        }

        const std::vector<business_logic::elements::structures::Port>& getOutputPorts()
            const override {
            return outputPorts;
        }

        business_logic::elements::blocks::BlockType getBlockType() const override {
            return business_logic::elements::blocks::BlockType::Subtract;
        }

       protected:
        static const std::vector<business_logic::elements::structures::Port> inputPorts;
        static const std::vector<business_logic::elements::structures::Port> outputPorts;
    };
}  // namespace business_logic::elements::blocks

#endif  // BUSINESS_LOGIC_ELEMENTS_IMPL_SUBTRACT_BLOCK_H
