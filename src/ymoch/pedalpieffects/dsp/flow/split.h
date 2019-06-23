#ifndef YMOCH_PEDALPIEFFECTS_DSP_FLOW_SPLIT_H_
#define YMOCH_PEDALPIEFFECTS_DSP_FLOW_SPLIT_H_

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::flow::split {

template <typename FunctionA, typename FunctionB>
inline type::Signal SplitMix(type::Signal in, FunctionA& a, FunctionB& b) {
  return a(in) + b(in);
}

}  // ymoch::pedalpyeffects::dsp::flow::split

#endif  // YMOCH_PEDALPIEFFECTS_DSP_FLOW_SPLIT_H_
