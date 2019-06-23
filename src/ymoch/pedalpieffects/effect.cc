#include "effect.h"

using ymoch::pedalpieffects::dsp::effect::biquad_filter::BiquadFilter;
using ymoch::pedalpieffects::dsp::effect::biquad_filter::HighPassFilter;
using ymoch::pedalpieffects::dsp::effect::biquad_filter::HighShelfFilter;
using ymoch::pedalpieffects::dsp::effect::biquad_filter::LowPassFilter;

namespace ymoch::pedalpieffects::effect {

namespace {

constexpr double kMinFrequencyHz = 5.0;

BiquadFilter DcCut(double sampling_rate_hz) {
  return HighPassFilter(sampling_rate_hz, kMinFrequencyHz);
}

}  // annonymous namespace

InputEqualizer::InputEqualizer(double sampling_rate_hz)
    : dc_cut_(DcCut(sampling_rate_hz)),
      high_boost_(HighShelfFilter(sampling_rate_hz, 1500, 12.0)) {}

LowFrequencyDriver::LowFrequencyDriver(double sampling_rate_hz)
    : xover_(LowPassFilter(sampling_rate_hz, 320)) {}

HighFrequencyDriver::HighFrequencyDriver(double sampling_rate_hz)
    : xover_(HighPassFilter(sampling_rate_hz, 640)) {}

}  // ymoch::pedalpieffects::effect
