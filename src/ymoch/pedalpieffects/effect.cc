#include "effect.h"

#include "dsp/effect/biquad-filter.h"
#include "dsp/flow/chain.h"
#include "dsp/flow/split.h"

namespace ymoch::pedalpieffects::effect {

using dsp::effect::biquad_filter::BiquadFilter;
using dsp::effect::biquad_filter::HighPassFilter;
using dsp::effect::biquad_filter::HighShelfFilter;
using dsp::effect::biquad_filter::LowPassFilter;
using dsp::flow::chain::Chain;
using dsp::type::Signal;

namespace {

constexpr double kMinFrequencyHz = 5.0;

class LowFrequencyDriver {
 public:
  explicit LowFrequencyDriver(double sampling_rate_hz);

  dsp::type::Signal operator()(dsp::type::Signal in) {
    return dsp::flow::chain::Chain(in, xover_);
  }

 private:
  dsp::effect::biquad_filter::BiquadFilter xover_;
};

class HighFrequencyDriver {
 public:
  explicit HighFrequencyDriver(double sampling_rate_hz);

  dsp::type::Signal operator()(dsp::type::Signal in) {
    return dsp::flow::chain::Chain(in, xover_);
  }

 private:
  dsp::effect::biquad_filter::BiquadFilter xover_;
};

class XoverDriver {
 public:
  explicit XoverDriver(LowFrequencyDriver&& low, HighFrequencyDriver&& high)
      : low_(std::move(low)), high_(std::move(high)) {}

  dsp::type::Signal operator()(dsp::type::Signal in) {
    return dsp::flow::split::SplitMix(in, low_, high_);
  }

 private:
  LowFrequencyDriver low_;
  HighFrequencyDriver high_;
};

BiquadFilter DcCut(double sampling_rate_hz) {
  return HighPassFilter(sampling_rate_hz, kMinFrequencyHz);
}

}  // annonymous namespace

class InputEqualizer::Impl {
 public:
  explicit Impl(double sampling_rate_hz)
    : dc_cut_(DcCut(sampling_rate_hz)),
      high_boost_(HighShelfFilter(sampling_rate_hz, 1500, 12.0)) {}

  Signal operator()(Signal in) {
    return Chain(in, dc_cut_, high_boost_);
  }

 private:
  BiquadFilter dc_cut_;
  BiquadFilter high_boost_;
};

InputEqualizer::InputEqualizer(double sampling_rate_hz)
    : impl_(new Impl(sampling_rate_hz)) {}
InputEqualizer::InputEqualizer(InputEqualizer&& other)
    : impl_(std::move(other.impl_)) {}
InputEqualizer::~InputEqualizer() = default;
Signal InputEqualizer::operator()(Signal in) {
  return (*impl_)(in);
}

LowFrequencyDriver::LowFrequencyDriver(double sampling_rate_hz)
    : xover_(LowPassFilter(sampling_rate_hz, 320)) {}

HighFrequencyDriver::HighFrequencyDriver(double sampling_rate_hz)
    : xover_(HighPassFilter(sampling_rate_hz, 640)) {}

class Effector::Impl {
 public:
  explicit Impl(double sampling_rate_hz)
      : xover_driver_(LowFrequencyDriver(sampling_rate_hz),
                      HighFrequencyDriver(sampling_rate_hz)) {}

  Signal operator()(Signal in) { return xover_driver_(in); }

 private:
  XoverDriver xover_driver_;
};

Effector::Effector(double sampling_rate_hz)
    : impl_(new Impl(sampling_rate_hz)) {}
Effector::~Effector() = default;
Signal Effector::operator()(Signal in) { return (*impl_)(in); }

}  // ymoch::pedalpieffects::effect
