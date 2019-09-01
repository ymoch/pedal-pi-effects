#include <cmath>

#include "biquad-filter.h"

namespace ymoch::pedalpieffects::dsp::effect::biquad_filter {

using type::Signal;

BiquadFilter::BiquadFilter(double a1, double a2, double b0, double b1,
                           double b2)
    : a1_(a1),
      a2_(a2),
      b0_(b0),
      b1_(b1),
      b2_(b2),
      in1_(0.0),
      in2_(0.0),
      out1_(0.0),
      out2_(0.0) {}

BiquadFilter::BiquadFilter(double a0, double a1, double a2, double b0,
                           double b1, double b2)
    // When given a0, normalize coefficients by a0.
    : BiquadFilter(a1 / a0, a2 / a0, b0 / a0, b1 / a0, b2 / a0) {}

Signal BiquadFilter::operator()(Signal in) {
  // See: https://en.wikipedia.org/wiki/Digital_biquad_filter#Direct_form_1
  const Signal out =
      b0_ * in + b1_ * in1_ + b2_ * in2_ - a1_ * out1_ - a2_ * out2_;

  in2_ = in1_;
  in1_ = in;
  out2_ = out1_;
  out1_ = out;

  return out;
}

BiquadFilter LowPassFilter(double sampling_rate, double frequency, double q) {
  const double omega = 2.0 * M_PI * frequency / sampling_rate;
  const double cos_omega = std::cos(omega);
  const double sin_omega = std::sin(omega);
  const double alpha = sin_omega / 2.0 / q;

  // clang-format off
  return BiquadFilter(
       1.0 + alpha,
      -2.0 * cos_omega,
       1.0 - alpha,
      (1.0 - cos_omega) / 2.0,
       1.0 - cos_omega,
      (1.0 - cos_omega) / 2.0
  );
  // clang-format on
}

BiquadFilter HighPassFilter(double sampling_rate, double frequency, double q) {
  const double omega = 2.0 * M_PI * frequency / sampling_rate;
  const double cos_omega = std::cos(omega);
  const double sin_omega = std::sin(omega);
  const double alpha = sin_omega / 2.0 / q;

  // clang-format off
  return BiquadFilter(
        1.0 + alpha,
       -2.0 * cos_omega,
        1.0 - alpha,
       (1.0 + cos_omega) / 2.0,
      -(1.0 + cos_omega),
       (1.0 + cos_omega) / 2.0
  );
  // clang-format on
}

BiquadFilter HighShelfFilter(double sampling_rate, double frequency,
                             double gain_db, double q) {
  const double omega = 2.0 * M_PI * frequency / sampling_rate;
  const double cos_omega = std::cos(omega);
  const double sin_omega = std::sin(omega);
  const double a = std::pow(10.0, gain_db / 40.0);
  const double alpha = sin_omega / 2.0 / q;
  const double beta = 2.0 * std::sqrt(a) * alpha;

  // clang-format off
  return BiquadFilter(
                  (a + 1.0) - (a - 1.0) * cos_omega + beta,
           2.0 * ((a - 1.0) - (a + 1.0) * cos_omega       ),
                  (a + 1.0) - (a - 1.0) * cos_omega - beta,
             a * ((a + 1.0) + (a - 1.0) * cos_omega + beta),
      -2.0 * a * ((a - 1.0) + (a + 1.0) * cos_omega       ),
             a * ((a + 1.0) + (a - 1.0) * cos_omega - beta)
  );
  // clang-format on
}

}  // ymoch::pedalpieffects::dsp::effect::biquad_filter
