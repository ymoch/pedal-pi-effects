#ifndef YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_
#define YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::flow::toggle {

template <typename Functor>
class Toggle {
 public:
  explicit Toggle(const Functor& functor) : Toggle(functor, false) {}

  Toggle(const Functor& functor, bool enabled)
      : functor_(functor), enabled_(enabled) {}

  bool enabled() const { return enabled_; }

  bool enabled(bool enabled) { return enabled_ = enabled; }

  type::Signal operator()(type::Signal signal) {
    const auto processed_signal = functor_(signal);
    if (enabled_) {
      return processed_signal;
    }
    return signal;
  }

 private:
  Functor functor_;
  bool enabled_;
};

template <typename T>
Toggle<T> MakeToggle(const T& functor) {
  return Toggle<T>(functor);
}

template <typename T>
Toggle<T> MakeToggle(const T& functor, bool enabled) {
  return Toggle<T>(functor, enabled);
}

}  // ymoch::pedalpieffects::dsp::flow::toggle

#endif  // YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_
