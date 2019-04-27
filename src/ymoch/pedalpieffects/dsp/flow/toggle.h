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

  type::Signal operator()(type::Signal signal) const {
    if (enabled_) {
      return functor_(signal);
    }
    return signal;
  }

 private:
  Functor functor_;
  bool enabled_;
};

}  // ymoch::pedalpieffects::dsp::flow::toggle

#endif  // YMOCH_PEDALPIEFFECTS_DSP_FLOW_TOGGLE_H_
