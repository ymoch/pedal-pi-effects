#include "ymoch/pedalpieffects/dsp/flow/chain.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ymoch/pedalpieffects/dsp/type.h"

using testing::DoubleEq;
using ymoch::pedalpieffects::dsp::type::Signal;

namespace ymoch::pedalpieffects::dsp::flow::chain {

TEST(Chain, ProcessesByEachFunctor) {
  auto chain = MakeChain([](Signal in) { return in + 1.0; },
                         [](Signal in) { return in * 2.0; },
                         [](Signal in) { return in - 1.0; });

  EXPECT_THAT(chain(-1.0), DoubleEq(-1.0));
  EXPECT_THAT(chain(0.0), DoubleEq(1.0));
  EXPECT_THAT(chain(1.0), DoubleEq(3.0));
  EXPECT_THAT(chain(2.0), DoubleEq(5.0));
}

}  // ymoch::pedalpieffects::dsp::flow::chain
