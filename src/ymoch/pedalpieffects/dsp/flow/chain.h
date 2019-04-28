#ifndef YMOCH_PEDALPIEFFECTS_DSP_FLOW_CHAIN_H_
#define YMOCH_PEDALPIEFFECTS_DSP_FLOW_CHAIN_H_

#include <tuple>

#include "ymoch/pedalpieffects/dsp/type.h"

namespace ymoch::pedalpieffects::dsp::flow::chain {

template <typename Head, typename... Tail>
class Chain {
 public:
  explicit Chain(const Head& head, const Tail&... tail)
      : head_(head), tail_chain_(tail...) {}

  type::Signal operator()(type::Signal in) { return tail_chain_(head_(in)); }

 private:
  Head head_;
  Chain<Tail...> tail_chain_;
};

template <typename Head>
class Chain<Head> {
 public:
  explicit Chain(const Head& head) : head_(head) {}

  type::Signal operator()(type::Signal in) { return head_(in); }

 private:
  Head head_;
};

template <typename Head, typename... Tail>
Chain<Head, Tail...> MakeChain(const Head& head, const Tail&... tail) {
  return Chain<Head, Tail...>(head, tail...);
}

}  // ymoch::pedalpieffects::dsp::flow::chain

#endif  // YMOCH_PEDALPIEFFECTS_DSP_FLOW_CHAIN_H_
