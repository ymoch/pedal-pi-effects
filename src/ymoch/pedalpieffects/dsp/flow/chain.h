#ifndef YMOCH_PEDALPIEFFECTS_DSP_FLOW_CHAIN_H_
#define YMOCH_PEDALPIEFFECTS_DSP_FLOW_CHAIN_H_

#include <tuple>

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::flow::chain {

inline type::Signal Chain(type::Signal in) { return in; }

template <typename Head, typename... Tail>
inline type::Signal Chain(type::Signal in, Head& head, Tail&... tail) {
  return Chain(head(in), tail...);
}

}  // ymoch::pedalpieffects::dsp::flow::chain

#endif  // YMOCH_PEDALPIEFFECTS_DSP_FLOW_CHAIN_H_
