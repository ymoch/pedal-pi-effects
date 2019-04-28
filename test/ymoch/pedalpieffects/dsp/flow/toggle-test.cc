#include "ymoch/pedalpieffects/dsp/flow/toggle.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ymoch/pedalpieffects/dsp/type.h"

using testing::_;
using testing::Eq;
using testing::DoubleEq;
using testing::Return;
using ymoch::pedalpieffects::dsp::type::Signal;

namespace ymoch::pedalpieffects::dsp::flow::toggle {

TEST(Toggle, ProcessesSignalsWhenEnabled) {
  const auto functor = [](Signal _) { return 1.0; };

  auto default_toggle = MakeToggle(functor);
  EXPECT_THAT(default_toggle.enabled(), Eq(false));
  EXPECT_THAT(default_toggle(0.0), DoubleEq(0.0));
  EXPECT_THAT(default_toggle(0.1), DoubleEq(0.1));

  auto toggle = MakeToggle(functor, true);
  EXPECT_THAT(toggle.enabled(), Eq(true));
  EXPECT_THAT(toggle(0.0), DoubleEq(1.0));
  EXPECT_THAT(toggle(0.1), DoubleEq(1.0));

  EXPECT_THAT(toggle.enabled(false), Eq(false));
  EXPECT_THAT(toggle(0.0), DoubleEq(0.0));
  EXPECT_THAT(toggle(0.1), DoubleEq(0.1));
}

TEST(Toggle, CallFunctorEvenWhenDisabled) {
  auto functor = testing::MockFunction<Signal(Signal signal)>();
  EXPECT_CALL(functor, Call(_)).WillOnce(Return(1.0));

  auto toggle = MakeToggle(functor.AsStdFunction(), false);
  EXPECT_THAT(toggle(0.0), DoubleEq(0.0));
}

}  // ymoch::pedalpieffects::dsp::effect::amplification
