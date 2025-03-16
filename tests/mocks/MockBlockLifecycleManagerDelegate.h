#ifndef MOCK_BLOCK_LIFECYCLE_MANAGER_DELEGATE_H
#define MOCK_BLOCK_LIFECYCLE_MANAGER_DELEGATE_H

#include <gmock/gmock.h>

#include "business_logic/delegate/IBlockLifecycleManagerDelegate.h"

class MockBlockLifecycleManagerDelegate
    : public business_logic::delegate::IBlockLifecycleManagerDelegate {
   public:
    MOCK_METHOD(bool,
                hasConnectionBetween,
                (const business_logic::elements::structures::BlocksConnectionSide&,
                 const business_logic::elements::structures::BlocksConnectionSide&),
                (const, override));
    MOCK_METHOD(bool,
                isInputConnected,
                (const business_logic::elements::structures::BlocksConnectionSide&),
                (const, override));
    MOCK_METHOD(void,
                onPortsConnected,
                (const business_logic::elements::structures::BlocksConnectionSide&,
                 const business_logic::elements::structures::BlocksConnectionSide&),
                (override));
    MOCK_METHOD(void,
                onBlockDeleted,
                (const business_logic::elements::blocks::BaseBlock*),
                (override));
};

#endif  // MOCK_BLOCK_LIFECYCLE_MANAGER_DELEGATE_H
