#ifndef YMOCH_PEDALPIEFFECTS_DSP_EFFECT_TUBE_CLIPPING_H_
#define YMOCH_PEDALPIEFFECTS_DSP_EFFECT_TUBE_CLIPPING_H_

#include <cmath>

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::effect::tube_clipping {

class TubeClipper {
 public:
  type::Signal operator()(type::Signal signal) const {
    signal = std::tanh(signal);
    if (signal < 0) {
      return signal * 0.8;
    }
    return signal;
  }
};

}  // ymoch::pedalpieffects::dsp::effect::tube_clipping

#endif  // YMOCH_PEDALPIEFFECTS_DSP_EFFECT_TUBE_CLIPPING_H_
