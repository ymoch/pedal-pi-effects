#ifndef YMOCH_PEDALPIEFFECTS_INPUT_EQUALIZATION_H_
#define YMOCH_PEDALPIEFFECTS_INPUT_EQUALIZATION_H_

#include <memory>

#include "dsp/type.h"

namespace ymoch::pedalpieffects::input_equalization {

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

}  // ymoch::pedalpieffects::effect

#endif  // YMOCH_PEDALPIEFFECTS_INPUT_EQUALIZATION_H_
