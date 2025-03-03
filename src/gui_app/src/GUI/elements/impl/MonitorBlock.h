#ifndef GUI_ELEMENTS_IMPL_MONITOR_BLOCK_H
#define GUI_ELEMENTS_IMPL_MONITOR_BLOCK_H

#include "GUI/elements/base/BaseBlock.h"
#include "GUI/elements/base/BlockType.h"
#include "GUI/renderer/FontManager.h"
#include "logging/Loggable.h"
#include "utils/misc.h"

namespace gui::elements::impl {
    /**
     * A monitor block that displays the value of its input port
     */
    class MonitorBlock : public business_logic::Loggable<MonitorBlock>,
                         public gui::elements::base::BaseBlock {
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
        MonitorBlock(
            int cx,
            int cy,
            gui::logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
            gui::logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
            const geometry::Size2D& windowSize);

        void render(SkCanvas* canvas, int mouseX, int mouseY, bool isHovered) override;

        std::string getSelfId() const override { return selfId; }

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
            return gui::elements::base::BlockType::Monitor;
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

#endif  // GUI_ELEMENTS_IMPL_MONITOR_BLOCK_H
