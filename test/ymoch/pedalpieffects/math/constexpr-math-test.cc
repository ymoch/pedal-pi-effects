#include "ymoch/pedalpieffects/math/constexpr-math.h"

#include <tuple>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Eq;

namespace ymoch::pedalpieffects::math::constexpr_math {

TEST(PowerTest, CreatesValueAsConstantExpression) {
  constexpr auto kZerothPowerOfZero = Power<0, 0>::value;
  EXPECT_THAT(kZerothPowerOfZero, Eq(1));

  constexpr auto kZerothPowerOfTwo = Power<2, 0>::value;
  EXPECT_THAT(kZerothPowerOfTwo, Eq(1));

  constexpr auto kSecondPowerOfZero = Power<0, 2>::value;
  EXPECT_THAT(kSecondPowerOfZero, Eq(0));

  constexpr auto kFirstPowerOfOne = Power<1, 1>::value;
  EXPECT_THAT(kFirstPowerOfOne, Eq(1));

  constexpr auto kTenthPowerOfTwo = Power<2, 10>::value;
  EXPECT_THAT(kTenthPowerOfTwo, Eq(1024));
}

}  // ymoch::pedalpieffects::constexpr_math
