#ifndef BUSINESS_LOGIC_ELEMENTS_IMPL_ADD_BLOCK_H
#define BUSINESS_LOGIC_ELEMENTS_IMPL_ADD_BLOCK_H

#include "elements/blocks/BaseBlock.h"
#include "logging/Loggable.h"
#include "typenames.h"
#include "utils/misc.h"

namespace business_logic::elements::blocks {
    /**
     * @brief A block that adds two input values together and outputs their sum
     */
    class AddBlock final : public business_logic::Loggable<AddBlock>,
                           public business_logic::elements::blocks::BaseBlock {
       public:
        AddBlock(
            int cx,
            int cy,
            business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            business_logic::delegate::IWindowDelegate* windowDelegate);

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override;

       protected:
        using business_logic::Loggable<AddBlock>::logger;

        std::string selfId;

        static constexpr int ADD_BLOCK_WIDTH = 120;
        static constexpr int ADD_BLOCK_HEIGHT = 120;

        const std::vector<business_logic::elements::structures::Port>& getInputPorts()
            const override {
            return inputPorts;
        }

        const std::vector<business_logic::elements::structures::Port>& getOutputPorts()
            const override {
            return outputPorts;
        }

        business_logic::elements::blocks::BlockType getBlockType() const override {
            return business_logic::elements::blocks::BlockType::Add;
        }

       protected:
        static const std::vector<business_logic::elements::structures::Port> inputPorts;
        static const std::vector<business_logic::elements::structures::Port> outputPorts;
    };
}  // namespace business_logic::elements::blocks

#endif  // BUSINESS_LOGIC_ELEMENTS_IMPL_ADD_BLOCK_H
