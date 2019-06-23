#include "ymoch/pedalpieffects/dsp/flow/split.h"

#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using std::bind;
using std::placeholders::_1;
using testing::DoubleEq;
using ymoch::pedalpieffects::dsp::type::Signal;

namespace ymoch::pedalpieffects::dsp::flow::split {

TEST(Split, ProcessAndMixByEachFunctor) {
  auto func1 = [](Signal in) { return in + 1.0; };
  auto func2 = [](Signal in) { return in * 2.0; };
  auto split_mix = bind<Signal(Signal, decltype(func1)&, decltype(func2)&)>(
      SplitMix, _1, func1, func2);

  EXPECT_THAT(split_mix(-1.0), DoubleEq(-2.0));
  EXPECT_THAT(split_mix(0.0), DoubleEq(1.0));
  EXPECT_THAT(split_mix(1.0), DoubleEq(4.0));
  EXPECT_THAT(split_mix(2.0), DoubleEq(7.0));
}

}  // ymoch::pedalpieffects::dsp::flow::split
