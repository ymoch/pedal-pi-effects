#ifndef YMOCH_PEDALPIEFFECTS_DSP_OSCILLATION_H_
#define YMOCH_PEDALPIEFFECTS_DSP_OSCILLATION_H_

#include <cmath>

#include "type.h"

namespace ymoch::pedalpieffects::dsp::oscillation {

class SineOscillator {
 public:
  type::Signal operator()() {
    const type::Signal signal = std::sin(phase_);
    Next();
    return signal;
  }

  static SineOscillator OfPeriod(double period) {
    return SineOscillator(kBasePeriod / period);
  }

  static SineOscillator OfSampling(double sampling_rate, double frequency) {
    return SineOscillator(kBasePeriod * frequency / sampling_rate);
  }

 private:
  static constexpr double kBasePeriod = 2.0 * M_PI;
  double step_;
  double phase_;

  explicit SineOscillator(double step) : step_(step), phase_(0.0) {}

  void Next() {
    phase_ += step_;
    while (phase_ > kBasePeriod) {
      phase_ -= kBasePeriod;  // To keep the resolution.
    }
  }
};

}  // namespace ymoch::pedalpieffects::dsp::oscillation

#endif  // YMOCH_PEDALPIEFFECTS_DSP_OSCILLATION_H_
