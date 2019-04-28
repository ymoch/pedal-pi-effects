#include "ymoch/pedalpieffects/dsp/effect/biquad-filter.h"

#include <cmath>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ymoch/pedalpieffects/dsp/type.h"

using testing::AllOf;
using testing::Gt;
using testing::Lt;

namespace ymoch::pedalpieffects::dsp::effect::biquad_filter {

namespace {
class SineOscillator {
 public:
  explicit SineOscillator(double period) : period_(period), phase_(0.0) {}

  SineOscillator(double sampling_rate, double frequency)
      : SineOscillator(kBasePeriod * frequency / sampling_rate) {}

  type::Signal operator()() {
    const type::Signal signal = std::sin(phase_);
    phase_ += period_;
    while (phase_ > kBasePeriod) {
      phase_ -= kBasePeriod;  // To keep the resolution.
    }
    return signal;
  }

 private:
  static constexpr double kBasePeriod = 2.0 * M_PI;
  double period_;
  double phase_;
};

}  // anonymous namespace

TEST(HighShelfFilter, DoesNotAffectLowFrequencyWaves) {
  constexpr double kSamplingRateHz = 2000;
  constexpr double kFrequencyHz = 50;
  SineOscillator osc(kSamplingRateHz, kFrequencyHz);

  constexpr double kFilterFrequencyHz = 500;
  BiquadFilter filter =
      HighShelfFilter(kSamplingRateHz, kFilterFrequencyHz, 1.0, 1.0);

  type::Signal raw_min = 0.0;
  type::Signal raw_max = 0.0;
  type::Signal filtered_min = 0.0;
  type::Signal filtered_max = 0.0;
  for (int i = 0; i < kSamplingRateHz * 10; ++i) {
    const type::Signal raw = osc();
    const type::Signal filtered = filter(raw);

    raw_min = std::min(raw, raw_min);
    raw_max = std::max(raw, raw_max);
    filtered_min = std::min(filtered, filtered_min);
    filtered_max = std::max(filtered, filtered_max);
  }

  ASSERT_THAT(raw_min, AllOf(Gt(-1.00001), Lt(-0.99)));
  ASSERT_THAT(raw_max, AllOf(Gt(0.99), Lt(1.00001)));
  EXPECT_THAT(filtered_min, AllOf(Gt(-1.01), Lt(-0.99)));
  EXPECT_THAT(filtered_max, AllOf(Gt(0.99), Lt(1.01)));
}

}  // ymoch::pedalpieffects::dsp::effect::biquad_filter
