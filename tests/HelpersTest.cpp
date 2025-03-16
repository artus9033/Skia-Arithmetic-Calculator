#include <gtest/gtest.h>

#include "business_logic/geometry/helpers.h"

using namespace business_logic::geometry;

TEST(IsCircleHoveredTest, InsideCircle) { EXPECT_TRUE(isCircleHovered(5, 5, 5, 5, 3)); }

TEST(IsCircleHoveredTest, OnBoundary) {
    EXPECT_TRUE(isCircleHovered(2, 5, 5, 5, 3));
    EXPECT_TRUE(isCircleHovered(8, 5, 5, 5, 3));
    EXPECT_TRUE(isCircleHovered(5, 2, 5, 5, 3));
    EXPECT_TRUE(isCircleHovered(5, 8, 5, 5, 3));
}

TEST(IsCircleHoveredTest, OutsideCircle) {
    EXPECT_FALSE(isCircleHovered(1, 1, 5, 5, 3));
    EXPECT_FALSE(isCircleHovered(10, 10, 5, 5, 3));
}

TEST(IsCircleHoveredTest, EdgeCases) {
    EXPECT_FALSE(isCircleHovered(1, 5, 5, 5, 3));  // Just outside the left boundary
    EXPECT_FALSE(isCircleHovered(9, 5, 5, 5, 3));  // Just outside the right boundary
    EXPECT_FALSE(isCircleHovered(5, 1, 5, 5, 3));  // Just outside the top boundary
    EXPECT_FALSE(isCircleHovered(5, 9, 5, 5, 3));  // Just outside the bottom boundary
}
