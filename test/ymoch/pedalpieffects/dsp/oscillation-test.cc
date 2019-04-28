#include "ymoch/pedalpieffects/dsp/oscillation.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::AllOf;
using testing::DoubleEq;
using testing::Gt;
using testing::Lt;

namespace ymoch::pedalpieffects::dsp::oscillation {

namespace {
const auto kCloseToZero = AllOf(Gt(-0.0000001), Lt(0.0000001));
const auto kCloseToPositiveOne = AllOf(Gt(0.9999999), Lt(1.0000001));
const auto kCloseToNegativeOne = AllOf(Gt(-1.0000001), Lt(-0.9999999));
}

TEST(SineOscillator, GeneratesSineWaveOfGivenPeriod) {
  auto osc = SineOscillator::OfPeriod(4);

  EXPECT_THAT(osc(), kCloseToZero);
  EXPECT_THAT(osc(), kCloseToPositiveOne);
  EXPECT_THAT(osc(), kCloseToZero);
  EXPECT_THAT(osc(), kCloseToNegativeOne);
  EXPECT_THAT(osc(), kCloseToZero);
}

TEST(SineOscillator, GeneratesSineWaveOfGivenSampling) {
  constexpr double kSamplingRate = 24.0;
  constexpr double kFrequency = 3.0;
  auto osc = SineOscillator::OfSampling(kSamplingRate, kFrequency);

  EXPECT_THAT(osc(), kCloseToZero);
  osc();
  EXPECT_THAT(osc(), kCloseToPositiveOne);
  osc();
  EXPECT_THAT(osc(), kCloseToZero);
  osc();
  EXPECT_THAT(osc(), kCloseToNegativeOne);
  osc();
  EXPECT_THAT(osc(), kCloseToZero);
}

}  // ymoch::pedalpieffects::dsp::oscillation
