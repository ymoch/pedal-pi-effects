#ifndef YMOCH_PEDALPIEFFECTS_MATH_SIGNAL_H_
#define YMOCH_PEDALPIEFFECTS_MATH_SIGNAL_H_

namespace ymoch::pedalpieffects::math::signal {

template <typename T>
class Normalizer {
 public:
  Normalizer(const T& lower_limit, const T& upper_limit)
      : lower_limit_(lower_limit),
        upper_limit_(upper_limit),
        lower_limit_double_(lower_limit_),
        upper_limit_double_(upper_limit_),
        width_((upper_limit_double_ - lower_limit_double_) / 2.0),
        offset_(lower_limit_double_ + width_) {}

  double Normalize(const T& signal) const {
    return (static_cast<double>(signal) - offset_) / width_;
  }

  double Unnormalize(double signal) const {
    const double unnormalized = signal * width_ + offset_;
    if (unnormalized < lower_limit_double_) {
      return lower_limit_;
    }
    if (unnormalized > upper_limit_double_) {
      return upper_limit_;
    }
    return static_cast<T>(unnormalized);
  }

 private:
  T lower_limit_;
  T upper_limit_;
  double lower_limit_double_;
  double upper_limit_double_;
  double width_;
  double offset_;
};

}  // ymoch::pedalpieffects::math::signal

#endif  // YMOCH_PEDALPIEFFECTS_MATH_SIGNAL_H_
