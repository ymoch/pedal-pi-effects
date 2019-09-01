#include "input-equalization.h"

#include "dsp/effect/biquad-filter.h"
#include "dsp/flow/chain.h"

namespace ymoch::pedalpieffects::input_equalization {

using dsp::effect::biquad_filter::BiquadFilter;
using dsp::effect::biquad_filter::HighPassFilter;
using dsp::effect::biquad_filter::HighShelfFilter;
using dsp::flow::chain::Chain;
using dsp::type::Signal;

namespace {

constexpr double kMinFrequencyHz = 5.0;

}  // anonymous namespace

class InputEqualizer::Impl {
 public:
  explicit Impl(double sampling_rate_hz)
      : dc_cut_(HighPassFilter(sampling_rate_hz, kMinFrequencyHz)),
        high_boost_(HighShelfFilter(sampling_rate_hz, 1500, 12.0)) {}

  Signal operator()(Signal in) { return Chain(in, dc_cut_, high_boost_); }

 private:
  BiquadFilter dc_cut_;
  BiquadFilter high_boost_;
};

InputEqualizer::InputEqualizer(double sampling_rate_hz)
    : impl_(new Impl(sampling_rate_hz)) {}
InputEqualizer::InputEqualizer(InputEqualizer&& other)
    : impl_(std::move(other.impl_)) {}
InputEqualizer::~InputEqualizer() = default;
Signal InputEqualizer::operator()(Signal in) { return (*impl_)(in); }

}  // ymoch::pedalpieffects::input_equalization
