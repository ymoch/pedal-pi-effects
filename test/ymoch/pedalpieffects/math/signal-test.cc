#include "ymoch/pedalpieffects/math/signal.h"

#include <cstdint>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::Eq;
using testing::DoubleEq;
using ymoch::pedalpieffects::math::signal::Normalizer;

TEST(NormalizerTest, NormalizesSignal) {
  const Normalizer<uint32_t> normalizer(8, 16);

  EXPECT_THAT(normalizer.Normalize(7), DoubleEq(-1.25));
  EXPECT_THAT(normalizer.Normalize(8), DoubleEq(-1.0));
  EXPECT_THAT(normalizer.Normalize(9), DoubleEq(-0.75));
  EXPECT_THAT(normalizer.Normalize(15), DoubleEq(0.75));
  EXPECT_THAT(normalizer.Normalize(16), DoubleEq(1.0));
  EXPECT_THAT(normalizer.Normalize(17), DoubleEq(1.25));
}

TEST(NormalizeTest, UnnormalizesSingal) {
  const Normalizer<uint32_t> normalizer(8, 16);

  EXPECT_THAT(normalizer.Unnormalize(-1.25), Eq(8)); // saturation
  EXPECT_THAT(normalizer.Unnormalize(-1.0), Eq(8));
  EXPECT_THAT(normalizer.Unnormalize(-0.75), Eq(9));
  EXPECT_THAT(normalizer.Unnormalize(0.75), Eq(15));
  EXPECT_THAT(normalizer.Unnormalize(1.0), Eq(16));
  EXPECT_THAT(normalizer.Unnormalize(1.25), Eq(16)); // saturation
}
