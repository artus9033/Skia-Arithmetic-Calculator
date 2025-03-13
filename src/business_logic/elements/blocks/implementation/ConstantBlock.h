#ifndef BUSINESS_LOGIC_ELEMENTS_IMPL_CONSTANT_BLOCK_H
#define BUSINESS_LOGIC_ELEMENTS_IMPL_CONSTANT_BLOCK_H

#include "elements/blocks/BaseBlock.h"
#include "elements/interactions/IDoubleClickable.h"
#include "logging/Loggable.h"
#include "typenames.h"
#include "utils/misc.h"

/**
 * @brief The concrete elements specialized implementations
 */
namespace business_logic::elements::blocks {
    /**
     * @brief A constant block that holds a constant value (scalar)
     *
     * Can be double-clicked to edit the value
     */
    class ConstantBlock final : public business_logic::Loggable<ConstantBlock>,
                                public business_logic::elements::blocks::BaseBlock,
                                public business_logic::elements::interactions::IDoubleClickable {
       public:
        /**
         * @brief Constructor
         * @param cx The center x coordinate where to place the center of the block
         * @param cy The center y coordinate where to place the center of the block
         * @param newBlockChoiceDelegate The delegate that is notified when a new block is chosen to
         * be added to the canvas
         * @param blockLifecycleManagerDelegate The delegate that manages the lifecycles of blocks
         * @param windowDelegate The delegate of the window
         */
        ConstantBlock(
            int cx,
            int cy,
            business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            business_logic::delegate::IWindowDelegate* windowDelegate);

        /**
         * \copydoc business_logic::elements::interactions::IDoubleClickable::onDoubleClick
         */
        void onDoubleClick(int x, int y) override;

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override {
            // do nothing
        }

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
        static constexpr int CONSTANT_BLOCK_WIDTH = 120;

        /**
         * The height of the block
         */
        static constexpr int CONSTANT_BLOCK_HEIGHT = 120;

        /**
         * \copydoc business_logic::elements::blocks::BaseBlock::getInputPorts
         */
        const std::vector<business_logic::elements::structures::Port>& getInputPorts()
            const override {
            return inputPorts;
        }

        /**
         * \copydoc business_logic::elements::blocks::BaseBlock::getOutputPorts
         */
        const std::vector<business_logic::elements::structures::Port>& getOutputPorts()
            const override {
            return outputPorts;
        }

        /**
         * \copydoc business_logic::elements::blocks::BaseBlock::getBlockType
         */
        business_logic::elements::blocks::BlockType getBlockType() const override {
            return business_logic::elements::blocks::BlockType::Constant;
        }

        /**
         * The input ports of the block
         */
        static const std::vector<business_logic::elements::structures::Port> inputPorts;

        /**
         * The output ports of the block
         */
        static const std::vector<business_logic::elements::structures::Port> outputPorts;
    };
}  // namespace business_logic::elements::blocks

#endif  // BUSINESS_LOGIC_ELEMENTS_IMPL_CONSTANT_BLOCK_H
