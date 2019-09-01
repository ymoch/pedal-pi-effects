#include "effect.h"

#include "dsp/effect/amplification.h"
#include "dsp/effect/biquad-filter.h"
#include "dsp/effect/tube-clipping.h"
#include "dsp/flow/chain.h"
#include "dsp/flow/split.h"

namespace ymoch::pedalpieffects::effect {

using dsp::effect::amplification::Amplifier;
using dsp::effect::biquad_filter::BiquadFilter;
using dsp::effect::biquad_filter::HighPassFilter;
using dsp::effect::biquad_filter::LowPassFilter;
using dsp::effect::tube_clipping::TubeClipper;
using dsp::flow::chain::Chain;
using dsp::flow::split::SplitMix;
using dsp::type::Signal;

namespace {

constexpr double kMinFrequencyHz = 5.0;
constexpr double kLowXoverHz = 320.0;
constexpr double kHighXoverHz = 640.0;

class LowFrequencyDriver {
 public:
  LowFrequencyDriver(double sampling_rate_hz, double xover_hz)
      : xover_(LowPassFilter(sampling_rate_hz, xover_hz)),
        clip_() {}

  Signal operator()(Signal in) {
    return Chain(in, xover_, clip_);
  }

 private:
  BiquadFilter xover_;
  const TubeClipper clip_;
};

class HighFrequencyDriver {
 public:
  HighFrequencyDriver(double sampling_rate_hz, double xover_hz)
      : xover_(HighPassFilter(sampling_rate_hz, xover_hz)),
        gain_(30.0),
        clip_(),
        volume_(0.5) {}

  Signal operator()(Signal in) {
    return Chain(in, xover_, gain_, clip_, volume_);
  }

 private:
  BiquadFilter xover_;
  const Amplifier gain_;
  const TubeClipper clip_;
  const Amplifier volume_;
};

class XoverDriver {
 public:
  explicit XoverDriver(LowFrequencyDriver&& low, HighFrequencyDriver&& high)
      : low_(std::move(low)), high_(std::move(high)) {}

  Signal operator()(Signal in) {
    return SplitMix(in, low_, high_);
  }

 private:
  LowFrequencyDriver low_;
  HighFrequencyDriver high_;
};

inline BiquadFilter DcCut(double sampling_rate_hz) {
  return HighPassFilter(sampling_rate_hz, kMinFrequencyHz);
}

}  // annonymous namespace

class Effector::Impl {
 public:
  explicit Impl(double sampling_rate_hz)
      : gain_(1.5),
        xover_driver_(LowFrequencyDriver(sampling_rate_hz, kLowXoverHz),
                      HighFrequencyDriver(sampling_rate_hz, kHighXoverHz)),
        dc_cut_(DcCut(sampling_rate_hz)),
        master_volume_(1.0 / 1.5) {}

  Amplifier& gain() { return gain_; }
  const Amplifier& gain() const { return gain_; }

  Signal operator()(Signal in) {
    return Chain(in, xover_driver_, master_volume_);
  }

 private:
  Amplifier gain_;
  XoverDriver xover_driver_;
  BiquadFilter dc_cut_;
  const Amplifier master_volume_;
};

Effector::Effector(double sampling_rate_hz)
    : impl_(new Impl(sampling_rate_hz)) {}
Effector::Effector(Effector&& other)
    : impl_(std::move(other.impl_)) {}
Effector::~Effector() = default;
Amplifier& Effector::gain() { return impl_->gain(); }
const Amplifier& Effector::gain() const { return impl_->gain(); }
Signal Effector::operator()(Signal in) { return (*impl_)(in); }

}  // ymoch::pedalpieffects::effect
