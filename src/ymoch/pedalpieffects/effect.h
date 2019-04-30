#ifndef YMOCH_PEDALPIEFFECTS_EFFECT_H_
#define YMOCH_PEDALPIEFFECTS_EFFECT_H_

#include "dsp/effect/biquad-filter.h"
#include "dsp/flow/chain.h"
#include "dsp/type.h"

namespace ymoch::pedalpieffects::effect {

class InputEqualizer {
 public:
  explicit InputEqualizer(double sampling_rate);

  dsp::type::Signal operator()(dsp::type::Signal in) {
    return dsp::flow::chain::Chain(in, dc_cut_, high_boost_);
  }

 private:
  dsp::effect::biquad_filter::BiquadFilter dc_cut_;
  dsp::effect::biquad_filter::BiquadFilter high_boost_;
};

}  // ymoch::pedalpieffects::effect

#endif  // YMOCH_PEDALPIEFFECTS_EFFECT_H_
