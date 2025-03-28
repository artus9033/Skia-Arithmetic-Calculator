#ifndef BUSINESS_LOGIC_ELEMENTS_IMPL_MONITOR_BLOCK_H
#define BUSINESS_LOGIC_ELEMENTS_IMPL_MONITOR_BLOCK_H

#include <iomanip>
#include <sstream>

#include "constants.h"
#include "elements/blocks/BaseBlock.h"
#include "elements/blocks/BlockType.h"
#include "logging/Loggable.h"
#include "utils/misc.h"

namespace business_logic::elements::blocks {
    /**
     * @brief A monitor block that displays the value of its input port
     *
     * When hovered, the displayed value will present all stored fraction digits of the input port.
     */
    class MonitorBlock final : public business_logic::Loggable<MonitorBlock>,
                               public business_logic::elements::blocks::BaseBlock {
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
        MonitorBlock(
            int cx,
            int cy,
            business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            business_logic::delegate::IWindowDelegate* windowDelegate);

        /**
         * @brief Wrapper that gets the value of the output port above the block; if the input port
         * is not connected, it will not be rendered
         * @param isHovered True if the block is hovered over, false otherwise
         * @return The value to render above the block or `std::nullopt` if the value should not be
         * rendered
         */
        std::optional<std::string> getValueToRenderAboveBlock(
            [[maybe_unused]] bool isHovered) override;

        std::string getSelfId() const override { return selfId; }

        void calculateOutputValues() override {
            // do nothing
        }

       protected:
        // since Loggable is a template base class, the compiler does not see Logger::logger in the
        // current scope; so as not to use this->logger explicitly each time, the below brings it to
        // the current scope explicitly
        using business_logic::Loggable<MonitorBlock>::logger;

        /**
         * The address of the block in memory, used for logging purposes
         */
        std::string selfId;

        /**
         * The width of the block
         */
        static constexpr int MONITOR_BLOCK_WIDTH = 120;

        /**
         * The height of the block
         */
        static constexpr int MONITOR_BLOCK_HEIGHT = 120;

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
            return business_logic::elements::blocks::BlockType::Monitor;
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

#endif  // BUSINESS_LOGIC_ELEMENTS_IMPL_MONITOR_BLOCK_H
