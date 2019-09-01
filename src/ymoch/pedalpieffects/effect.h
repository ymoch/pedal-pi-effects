#ifndef YMOCH_PEDALPIEFFECTS_EFFECT_H_
#define YMOCH_PEDALPIEFFECTS_EFFECT_H_

#include <memory>

#include "dsp/effect/biquad-filter.h"
#include "dsp/flow/chain.h"
#include "dsp/flow/split.h"
#include "dsp/type.h"

namespace ymoch::pedalpieffects::effect {

class InputEqualizer {
 public:
  explicit InputEqualizer(double sampling_rate_hz);
  InputEqualizer(InputEqualizer&& other);
  ~InputEqualizer();
  dsp::type::Signal operator()(dsp::type::Signal in);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

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

class Effector {
 public:
  explicit Effector(double sampling_rate_hz);
  ~Effector();

  dsp::type::Signal operator()(dsp::type::Signal in);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // ymoch::pedalpieffects::effect

#endif  // YMOCH_PEDALPIEFFECTS_EFFECT_H_
