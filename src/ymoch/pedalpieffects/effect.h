#ifndef YMOCH_PEDALPIEFFECTS_EFFECT_H_
#define YMOCH_PEDALPIEFFECTS_EFFECT_H_

#include "dsp/effect/biquad-filter.h"
#include "dsp/flow/chain.h"
#include "dsp/flow/split.h"
#include "dsp/type.h"

namespace ymoch::pedalpieffects::effect {

class InputEqualizer {
 public:
  explicit InputEqualizer(double sampling_rate_hz);

  dsp::type::Signal operator()(dsp::type::Signal in) {
    return dsp::flow::chain::Chain(in, dc_cut_, high_boost_);
  }

 private:
  dsp::effect::biquad_filter::BiquadFilter dc_cut_;
  dsp::effect::biquad_filter::BiquadFilter high_boost_;
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

}  // ymoch::pedalpieffects::effect

#endif  // YMOCH_PEDALPIEFFECTS_EFFECT_H_
