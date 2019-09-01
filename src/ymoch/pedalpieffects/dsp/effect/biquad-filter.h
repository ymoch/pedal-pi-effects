#ifndef YMOCH_PEDALPIEFFECTS_DSP_EFFECT_BIQUAD_FILTER_H_
#define YMOCH_PEDALPIEFFECTS_DSP_EFFECT_BIQUAD_FILTER_H_

#include <memory>

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::effect::biquad_filter {

constexpr double kDefaultQ = 1.41421356237 / 2.0;  // 1 / sqrt(2) = sqrt(2) / 2

class BiquadFilter {
 public:
  BiquadFilter(double a1, double a2, double b0, double b1, double b2);
  BiquadFilter(double a0, double a1, double a2, double b0, double b1,
               double b2);
  BiquadFilter(BiquadFilter&& other);
  ~BiquadFilter();
  type::Signal operator()(type::Signal in);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

BiquadFilter LowPassFilter(double sampling_rate, double frequency,
                           double q = kDefaultQ);

BiquadFilter HighPassFilter(double sampling_rate, double frequency,
                            double q = kDefaultQ);

BiquadFilter HighShelfFilter(double sampling_rate, double frequency,
                             double gain_db, double q = kDefaultQ);

}  // ymoch::pedalpieffects::dsp::effect::biquad_filter

#endif  // YMOCH_PEDALPIEFFECTS_DSP_EFFECT_BIQUAD_FILTER_H_
