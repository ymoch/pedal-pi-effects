#include "ymoch/pedalpieffects/dsp/effect/amplification.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::DoubleEq;

namespace ymoch::pedalpieffects::dsp::effect::amplification {

TEST(Amplifier, AmplifiesSignal) {
  const Amplifier default_amplifier;

  EXPECT_THAT(default_amplifier.factor(), DoubleEq(1.0));
  EXPECT_THAT(default_amplifier(-0.1), DoubleEq(-0.1));
  EXPECT_THAT(default_amplifier(0.0), DoubleEq(0.0));
  EXPECT_THAT(default_amplifier(0.1), DoubleEq(0.1));

  Amplifier amplifier(2.0);
  EXPECT_THAT(amplifier.factor(), DoubleEq(2.0));
  EXPECT_THAT(amplifier(-0.1), DoubleEq(-0.2));
  EXPECT_THAT(amplifier(0.0), DoubleEq(0.0));
  EXPECT_THAT(amplifier(0.1), DoubleEq(0.2));

  EXPECT_THAT(amplifier.factor(0.5), DoubleEq(0.5));
  EXPECT_THAT(amplifier.factor(), DoubleEq(0.5));
  EXPECT_THAT(amplifier(-0.1), DoubleEq(-0.05));
  EXPECT_THAT(amplifier(0.0), DoubleEq(0.0));
  EXPECT_THAT(amplifier(0.1), DoubleEq(0.05));
}

}  // ymoch::pedalpieffects::dsp::effect::amplification
