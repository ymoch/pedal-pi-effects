#include "ymoch/pedalpieffects/dsp/flow/chain.h"

#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ymoch/pedalpieffects/dsp/type.h"

using std::placeholders::_1;
using testing::DoubleEq;
using ymoch::pedalpieffects::dsp::type::Signal;

namespace ymoch::pedalpieffects::dsp::flow::chain {

TEST(Chain, ProcessesByEachFunctor) {
  auto func1 = [](Signal in) { return in + 1.0; };
  auto func2 = [](Signal in) { return in * 2.0; };
  auto func3 = [](Signal in) { return in - 1.0; };
  auto chain =
      std::bind<Signal(Signal, decltype(func1)&, decltype(func2)&,
                       decltype(func3)&)>(Chain, _1, func1, func2, func3);

  EXPECT_THAT(chain(-1.0), DoubleEq(-1.0));
  EXPECT_THAT(chain(0.0), DoubleEq(1.0));
  EXPECT_THAT(chain(1.0), DoubleEq(3.0));
  EXPECT_THAT(chain(2.0), DoubleEq(5.0));
}

}  // ymoch::pedalpieffects::dsp::flow::chain
