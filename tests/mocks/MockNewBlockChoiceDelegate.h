#ifndef MOCK_BLOCK_CHOICE_DELEGATE_H
#define MOCK_BLOCK_CHOICE_DELEGATE_H

#include <gmock/gmock.h>

#include "business_logic/delegate/INewBlockChoiceDelegate.h"

class MockNewBlockChoiceDelegate : public business_logic::delegate::INewBlockChoiceDelegate {
   public:
    MOCK_METHOD(void,
                onNewBlockChoice,
                (const business_logic::elements::blocks::BlockType& blockType),
                (override));
};

#endif  // MOCK_BLOCK_CHOICE_DELEGATE_H
