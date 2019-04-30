#include "ymoch/pedalpieffects/dsp/effect/biquad-filter.h"

#include <cmath>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ymoch/pedalpieffects/dsp/oscillation.h"
#include "ymoch/pedalpieffects/dsp/type.h"

using testing::AllOf;
using testing::Gt;
using testing::Lt;
using ymoch::pedalpieffects::dsp::oscillation::SineOscillator;

namespace ymoch::pedalpieffects::dsp::effect::biquad_filter {

namespace {

constexpr double kSamplingRateHz = 44100;
constexpr double kNumPeriod = 20;
constexpr double kTolerance = 0.1;

auto CloseTo(double expected) -> decltype(AllOf(Gt(0.0), Lt(0.0))) {
  return AllOf(Gt(expected - kTolerance), Lt(expected + kTolerance));
}

template <typename T>
void TestSineWaveAmplitude(T& filter, double frequency_hz, double expected_min,
                           double expected_max) {
  auto osc = SineOscillator::OfSampling(kSamplingRateHz, frequency_hz);

  type::Signal raw_min = 0.0;
  type::Signal raw_max = 0.0;
  type::Signal filtered_min = 0.0;
  type::Signal filtered_max = 0.0;
  constexpr int kNumSample = kSamplingRateHz * kNumPeriod;
  for (int i = 0; i < kNumSample; ++i) {
    const type::Signal raw = osc();
    const type::Signal filtered = filter(raw);

    raw_min = std::min(raw, raw_min);
    raw_max = std::max(raw, raw_max);
    filtered_min = std::min(filtered, filtered_min);
    filtered_max = std::max(filtered, filtered_max);
  }

  ASSERT_THAT(raw_min, CloseTo(-1.0));
  ASSERT_THAT(raw_max, CloseTo(1.0));
  EXPECT_THAT(filtered_min, CloseTo(expected_min));
  EXPECT_THAT(filtered_max, CloseTo(expected_max));
}

}  // annonymous namespace

TEST(LowPassFilter, DoesNotAffectLowFrequencyWaves) {
  constexpr double kFilterFrequencyHz = 400.0;
  auto filter = LowPassFilter(kSamplingRateHz, kFilterFrequencyHz);
  TestSineWaveAmplitude(filter, kFilterFrequencyHz / 16.0, -1.0, 1.0);
}

TEST(HighPassFilter, DoesNotAffectHighFrequencyWaves) {
  constexpr double kFilterFrequencyHz = 400.0;
  auto filter = HighPassFilter(kSamplingRateHz, kFilterFrequencyHz);
  TestSineWaveAmplitude(filter, kFilterFrequencyHz * 16.0, -1.0, 1.0);
}

TEST(HighShelfFilter, DoesNotAffectLowFrequencyWaves) {
  constexpr double kFilterFrequencyHz = 400.0;
  auto filter = HighShelfFilter(kSamplingRateHz, kFilterFrequencyHz, 3.0);
  TestSineWaveAmplitude(filter, kFilterFrequencyHz / 16.0, -1.0, 1.0);
}

}  // ymoch::pedalpieffects::dsp::effect::biquad_filter
