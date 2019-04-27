#ifndef YMOCH_PEDALPIEFFECTS_DSP_NORMALIZATION_H_
#define YMOCH_PEDALPIEFFECTS_DSP_NORMALIZATION_H_

#include "type.h"

namespace ymoch::pedalpieffects::dsp::normalization {

template <typename T>
class Normalizer {
 public:
  Normalizer(const T& lower_limit, const T& upper_limit)
      : lower_limit_(lower_limit),
        upper_limit_(upper_limit),
        lower_limit_as_signal_(lower_limit_),
        upper_limit_as_signal_(upper_limit_),
        width_((upper_limit_as_signal_ - lower_limit_as_signal_) / 2.0),
        offset_(lower_limit_as_signal_ + width_) {}

  type::Signal Normalize(const T& signal) const {
    return (static_cast<type::Signal>(signal) - offset_) / width_;
  }

  T Unnormalize(type::Signal signal) const {
    const type::Signal unnormalized = signal * width_ + offset_;
    if (unnormalized < lower_limit_as_signal_) {
      return lower_limit_;
    }
    if (unnormalized > upper_limit_as_signal_) {
      return upper_limit_;
    }
    return static_cast<T>(unnormalized);
  }

 private:
  T lower_limit_;
  T upper_limit_;
  type::Signal lower_limit_as_signal_;
  type::Signal upper_limit_as_signal_;
  type::Signal width_;
  type::Signal offset_;
};

}  // ymoch::pedalpieffects::math::signal

#endif  // YMOCH_PEDALPIEFFECTS_DSP_NORMALIZATION_H_
