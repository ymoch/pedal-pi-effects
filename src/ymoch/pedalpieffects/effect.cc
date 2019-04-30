#include "effect.h"

using ymoch::pedalpieffects::dsp::effect::biquad_filter::HighPassFilter;
using ymoch::pedalpieffects::dsp::effect::biquad_filter::HighShelfFilter;

namespace ymoch::pedalpieffects::effect {

namespace {
constexpr double kMinFrequencyHz = 5.0;
}  // annonymous namespace

InputEqualizer::InputEqualizer(double sampling_rate)
    : dc_cut_(HighPassFilter(sampling_rate, kMinFrequencyHz)),
      high_boost_(HighShelfFilter(sampling_rate, 1500, 12.0)) {}

}  // ymoch::pedalpieffects::effect
