#ifndef YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_
#define YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_

#include <utility>

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::flow::toggle {

template <typename T>
class Toggle {
 public:
  explicit Toggle(T&& effect, bool enabled = false)
      : effect_(std::move(effect)), enabled_(enabled) {}

  bool enabled() const { return enabled_; }

  bool enabled(bool enabled) { return enabled_ = enabled; }

  T& effect() { return effect_; }

  const T& effect() const { return effect_; }

  type::Signal operator()(type::Signal signal) {
    const auto processed_signal = effect_(signal);
    if (enabled_) {
      return processed_signal;
    }
    return signal;
  }

 private:
  T effect_;
  bool enabled_;
};

template <typename T>
Toggle<T> MakeToggle(T&& effect, bool enabled = false) {
  return Toggle<T>(std::move(effect), enabled);
}

}  // ymoch::pedalpieffects::dsp::flow::toggle

#endif  // YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_
