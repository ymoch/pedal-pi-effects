#ifndef YMOCH_PEDALPIEFFECTS_EFFECT_H_
#define YMOCH_PEDALPIEFFECTS_EFFECT_H_

#include <memory>

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
