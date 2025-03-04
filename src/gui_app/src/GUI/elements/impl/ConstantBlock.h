#ifndef GUI_ELEMENTS_IMPL_CONSTANT_BLOCK_H
#define GUI_ELEMENTS_IMPL_CONSTANT_BLOCK_H

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/elements/base/IDoubleClickable.h"
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
         * @brief Constructor
         * @param cx The center x coordinate where to place the center of the block
         * @param cy The center y coordinate where to place the center of the block
         * @param newBlockChoiceDelegate The delegate that is notified when a new block is chosen to
         * be added to the canvas
         * @param blockLifecycleManagerDelegate The delegate that manages the lifecycles of blocks
         * @param windowSize The size of the window
         */
        ConstantBlock(
            int cx,
            int cy,
            gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            const geometry::Size2D& windowSize);

        void render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) override;

        /**
         * \copydoc IDoubleClickable::onDoubleClick
         */
        void onDoubleClick(int x, int y) override;

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override {
            // do nothing
            portValues[&outputPorts[0]] = boost::multiprecision::cpp_dec_float_50(1.05);
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
         * \copydoc BaseBlock::getInputPorts
         */
        const std::vector<gui::elements::base::Port>& getInputPorts() const override {
            return inputPorts;
        }

        /**
         * \copydoc BaseBlock::getOutputPorts
         */
        const std::vector<gui::elements::base::Port>& getOutputPorts() const override {
            return outputPorts;
        }

        /**
         * \copydoc BaseBlock::getBlockType
         */
        gui::elements::base::BlockType getBlockType() const override {
            return gui::elements::base::BlockType::Constant;
        }

       protected:
        /**
         * The input ports of the block
         */
        static const std::vector<gui::elements::base::Port> inputPorts;

        /**
         * The output ports of the block
         */
        static const std::vector<gui::elements::base::Port> outputPorts;
    };
}  // namespace gui::elements::impl

#endif  // GUI_ELEMENTS_IMPL_CONSTANT_BLOCK_H
