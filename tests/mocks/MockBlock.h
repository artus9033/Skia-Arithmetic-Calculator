#ifndef MOCK_BLOCK_H
#define MOCK_BLOCK_H

#include <gmock/gmock.h>

#include "business_logic/elements/blocks/BaseBlock.h"

class MockBlock : public business_logic::elements::blocks::BaseBlock {
   public:
    MockBlock(
        business_logic::delegate::INewBlockChoiceDelegate* newBlockChoiceDelegate,
        // NOLINTBEGIN
        business_logic::delegate::IBlockLifecycleManagerDelegate* blockLifecycleManagerDelegate,
        // NOLINTEND
        business_logic::delegate::IWindowDelegate* windowDelegate,
        std::shared_ptr<spdlog::logger> logger)
        : business_logic::elements::blocks::BaseBlock(0,
                                                      0,
                                                      10,
                                                      10,
                                                      newBlockChoiceDelegate,
                                                      blockLifecycleManagerDelegate,
                                                      windowDelegate,
                                                      logger) {};

    MOCK_METHOD(std::string, getSelfId, (), (const, override));
    MOCK_METHOD(business_logic::elements::blocks::BlockType, getBlockType, (), (const, override));
    MOCK_METHOD(void, calculateOutputValues, (), (override));
    MOCK_METHOD(const std::vector<business_logic::elements::structures::Port>&,
                getInputPorts,
                (),
                (const, override));
    MOCK_METHOD(const std::vector<business_logic::elements::structures::Port>&,
                getOutputPorts,
                (),
                (const, override));
};

#endif  // MOCK_BLOCK_H
