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

const double kTolerance = 0.0001;

auto CloseTo(double expected) -> decltype(AllOf(Gt(0.0), Lt(0.0))) {
  return AllOf(Gt(expected - kTolerance), Lt(expected + kTolerance));
}

template <typename T>
void TestSineWaveAmplitude(T& filter, double sampling_rate_hz,
                           double frequency_hz, double expected_min,
                           double expected_max) {
  auto osc = SineOscillator::OfSampling(sampling_rate_hz, frequency_hz);

  const int num_sample = static_cast<int>(sampling_rate_hz) * 10;
  type::Signal raw_min = 0.0;
  type::Signal raw_max = 0.0;
  type::Signal filtered_min = 0.0;
  type::Signal filtered_max = 0.0;
  for (int i = 0; i < num_sample; ++i) {
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

TEST(HighShelfFilter, DoesNotAffectLowFrequencyWaves) {
  constexpr double kSamplingRateHz = 2000;
  constexpr double kFrequencyHz = 50;
  constexpr double kFilterFrequencyHz = 500;

  BiquadFilter filter =
      HighShelfFilter(kSamplingRateHz, kFilterFrequencyHz, 1.0);
  TestSineWaveAmplitude(filter, kSamplingRateHz, kFrequencyHz, -1.0, 1.0);
}

/* TODO: Make valid tests.
TEST(HighShelfFilter, AffectsHighFrequencyWaves) {
  constexpr double kSamplingRateHz = 10000;
  constexpr double kFrequencyHz = 50;
  constexpr double kFilterFrequencyHz = 50;

  BiquadFilter filter =
      HighShelfFilter(kSamplingRateHz, kFilterFrequencyHz, 20.0);
  TestSineWaveAmplitude(filter, kSamplingRateHz, kFrequencyHz, -1.0, 1.0);
}
*/

}  // ymoch::pedalpieffects::dsp::effect::biquad_filter
