#ifndef MOCK_WINDOW_DELEGATE_H
#define MOCK_WINDOW_DELEGATE_H

#include <gmock/gmock.h>

#include "business_logic/delegate/IWindowDelegate.h"

class MockWindowDelegate : public business_logic::delegate::IWindowDelegate {
   public:
    MOCK_METHOD(void, showWarning, (const std::string&, const std::string&), (override));
    MOCK_METHOD(business_logic::geometry::Size2D, getWindowSize, (), (override));
    MOCK_METHOD(business_logic::geometry::Size2D, getFramebufferSize, (), (override));
    MOCK_METHOD(void, focusWindow, (), (override));
    MOCK_METHOD(std::optional<FloatingPoint>,
                promptFloatingPointInput,
                (const std::string&, const std::string&, const std::optional<FloatingPoint>&),
                (override));
    MOCK_METHOD(bool, promptConfirmation, (const std::string&, const std::string&), (override));
};

#endif  // MOCK_WINDOW_DELEGATE_H
