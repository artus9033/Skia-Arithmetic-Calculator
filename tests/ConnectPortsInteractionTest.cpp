/**
 * @file ConnectPortsInteractionTest.cpp
 * @brief Unit tests ConnectPortsInteractionTest class
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockBlock.h"
#include "MockBlockLifecycleManagerDelegate.h"
#include "MockNewBlockChoiceDelegate.h"
#include "MockWindowDelegate.h"
#include "business_logic/elements/blocks/structures/Port.h"
#include "business_logic/input/ConnectPortsInteraction.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

class ConnectPortsInteractionTest : public ::testing::Test {
   protected:
    business_logic::input::ConnectPortsInteraction interaction;
    business_logic::elements::structures::Port port1, port2;

    std::shared_ptr<spdlog::logger> logger = spdlog::default_logger();

    // behaviour of the below utility dependencies will not be tested, so NiceMock silences
    // "uninteresting call" warnings
    NiceMock<MockWindowDelegate> windowDelegate;

    NiceMock<MockBlockLifecycleManagerDelegate> blockLifecycleManagerDelegate;
    NiceMock<MockNewBlockChoiceDelegate> newBlockChoiceDelegate;

    NiceMock<MockBlock> block1 = {
        &newBlockChoiceDelegate, &blockLifecycleManagerDelegate, &windowDelegate, logger};
    NiceMock<MockBlock> block2 = {
        &newBlockChoiceDelegate, &blockLifecycleManagerDelegate, &windowDelegate, logger};

    void SetUp() override {
        port1.type = business_logic::elements::structures::Port::Type::OUTPUT;
        port2.type = business_logic::elements::structures::Port::Type::INPUT;
    };
};

TEST_F(ConnectPortsInteractionTest, InteractionStartsCorrectly) {
    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    EXPECT_TRUE(interaction.isStarted());
    EXPECT_TRUE(interaction.getStartSide().has_value());
    EXPECT_EQ(interaction.getStartSide()->block, &block1);
    EXPECT_EQ(interaction.getStartSide()->port, &port1);
}

TEST_F(ConnectPortsInteractionTest, ValidConnectionCompletes) {
    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    interaction.handleUserInteractedWith(
        &block2, &port2, &windowDelegate, &blockLifecycleManagerDelegate);

    EXPECT_FALSE(interaction.isStarted());
    EXPECT_FALSE(interaction.getStartSide().has_value());
    EXPECT_FALSE(interaction.getEndSide().has_value());
}

TEST_F(ConnectPortsInteractionTest, InvalidConnectionSameBlock) {
    EXPECT_CALL(windowDelegate, showWarning(_, _)).Times(1);

    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    interaction.handleUserInteractedWith(
        &block1, &port2, &windowDelegate, &blockLifecycleManagerDelegate);
}

TEST_F(ConnectPortsInteractionTest, InvalidConnectionDuplicate) {
    EXPECT_CALL(blockLifecycleManagerDelegate, hasConnectionBetween(_, _)).WillOnce(Return(true));
    EXPECT_CALL(windowDelegate, showWarning(_, _)).Times(1);

    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    interaction.handleUserInteractedWith(
        &block2, &port2, &windowDelegate, &blockLifecycleManagerDelegate);
}

TEST_F(ConnectPortsInteractionTest, InvalidConnectionInputOccupied) {
    EXPECT_CALL(blockLifecycleManagerDelegate, hasConnectionBetween(_, _)).WillOnce(Return(false));
    EXPECT_CALL(blockLifecycleManagerDelegate, isInputConnected(_)).WillOnce(Return(true));
    EXPECT_CALL(windowDelegate, showWarning(_, _)).Times(1);

    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    interaction.handleUserInteractedWith(
        &block2, &port2, &windowDelegate, &blockLifecycleManagerDelegate);
}

TEST_F(ConnectPortsInteractionTest, ValidConnection) {
    EXPECT_CALL(blockLifecycleManagerDelegate, hasConnectionBetween(_, _)).WillOnce(Return(false));
    EXPECT_CALL(blockLifecycleManagerDelegate, isInputConnected(_)).WillOnce(Return(false));
    EXPECT_CALL(blockLifecycleManagerDelegate, onPortsConnected(_, _)).Times(1);

    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    interaction.handleUserInteractedWith(
        &block2, &port2, &windowDelegate, &blockLifecycleManagerDelegate);
}

TEST_F(ConnectPortsInteractionTest, ResetInteraction) {
    interaction.handleUserInteractedWith(
        &block1, &port1, &windowDelegate, &blockLifecycleManagerDelegate);
    interaction.resetInteraction();
    EXPECT_FALSE(interaction.isStarted());
    EXPECT_FALSE(interaction.getStartSide().has_value());
    EXPECT_FALSE(interaction.getEndSide().has_value());
}
