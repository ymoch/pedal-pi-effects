#ifndef YMOCH_PEDALPIEFFECTS_DSP_EFFECT_AMPLIFICATION_H_
#define YMOCH_PEDALPIEFFECTS_DSP_EFFECT_AMPLIFICATION_H_

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::effect::amplification {

class Amplifier {
 public:
  Amplifier() : Amplifier(1.0) {}
  explicit Amplifier(double factor) : factor_(factor) {}

  double factor() const { return factor_; }

  double factor(double factor) { return factor_ = factor; }

  type::Signal operator()(type::Signal signal) const {
    return signal * factor_;
  }

 private:
  double factor_;
};

}  // ymoch::pedalpieffects::dsp::effect::amplification

#endif  // YMOCH_PEDALPIEFFECTS_DSP_EFFECT_AMPLIFICATION_H_
