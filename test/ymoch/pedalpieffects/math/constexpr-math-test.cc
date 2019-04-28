#include "ymoch/pedalpieffects/math/constexpr-math.h"

#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Eq;

namespace ymoch::pedalpieffects::math::constexpr_math {

TEST(PowerTest, CreatesValueAsConstantExpression) {
  EXPECT_THAT(power(0, 0), Eq(1));
  EXPECT_THAT(power(2, 0), Eq(1));
  EXPECT_THAT(power(0, 2), Eq(0));
  EXPECT_THAT(power(1, 1), Eq(1));
  EXPECT_THAT(power(2, 10), Eq(1024));
}

}  // ymoch::pedalpieffects::constexpr_math
