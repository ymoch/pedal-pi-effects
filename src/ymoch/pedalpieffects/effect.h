#ifndef YMOCH_PEDALPIEFFECTS_EFFECT_H_
#define YMOCH_PEDALPIEFFECTS_EFFECT_H_

#include <memory>

#include "dsp/effect/amplification.h"
#include "dsp/type.h"

namespace ymoch::pedalpieffects::effect {

class Effector {
 public:
  explicit Effector(double sampling_rate_hz);
  Effector(Effector&& other);
  ~Effector();

  dsp::effect::amplification::Amplifier& gain();
  const dsp::effect::amplification::Amplifier& gain() const;

  dsp::type::Signal operator()(dsp::type::Signal in);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // ymoch::pedalpieffects::effect

#endif  // YMOCH_PEDALPIEFFECTS_EFFECT_H_
