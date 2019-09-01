#include "effect.h"

#include "dsp/effect/amplification.h"
#include "dsp/effect/biquad-filter.h"
#include "dsp/flow/chain.h"
#include "dsp/flow/split.h"

namespace ymoch::pedalpieffects::effect {

using dsp::effect::amplification::Amplifier;
using dsp::effect::biquad_filter::BiquadFilter;
using dsp::effect::biquad_filter::HighPassFilter;
using dsp::effect::biquad_filter::HighShelfFilter;
using dsp::effect::biquad_filter::LowPassFilter;
using dsp::flow::chain::Chain;
using dsp::type::Signal;

namespace {

constexpr double kMinFrequencyHz = 5.0;
constexpr double kLowXoverFrequencyHz = 320.0;
constexpr double kHighXoverFrequencyHz = 640.0;

class LowFrequencyDriver {
 public:
  explicit LowFrequencyDriver(double sampling_rate_hz)
    : xover_(LowPassFilter(sampling_rate_hz, kLowXoverFrequencyHz)) {}

  dsp::type::Signal operator()(dsp::type::Signal in) {
    return dsp::flow::chain::Chain(in, xover_);
  }

 private:
  dsp::effect::biquad_filter::BiquadFilter xover_;
};

class HighFrequencyDriver {
 public:
  explicit HighFrequencyDriver(double sampling_rate_hz)
    : xover_(HighPassFilter(sampling_rate_hz, kHighXoverFrequencyHz)) {}

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

class Effector::Impl {
 public:
  explicit Impl(double sampling_rate_hz)
      : gain_(1.5),
        xover_driver_(LowFrequencyDriver(sampling_rate_hz),
                      HighFrequencyDriver(sampling_rate_hz)),
        master_volume_(1.0 / 1.5) {}

  Amplifier& gain() { return gain_; }
  const Amplifier& gain() const { return gain_; }

  Signal operator()(Signal in) {
    return Chain(in, xover_driver_, master_volume_);
  }

 private:
  Amplifier gain_;
  XoverDriver xover_driver_;
  const Amplifier master_volume_;
};

Effector::Effector(double sampling_rate_hz)
    : impl_(new Impl(sampling_rate_hz)) {}
Effector::~Effector() = default;
Amplifier& Effector::gain() {
  return impl_->gain();
}
const Amplifier& Effector::gain() const {
  return impl_->gain();
}
Signal Effector::operator()(Signal in) {
  return (*impl_)(in);
}

}  // ymoch::pedalpieffects::effect
