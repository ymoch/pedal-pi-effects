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
  auto default_toggle = MakeToggle([](Signal _) { return 1.0; });
  EXPECT_THAT(default_toggle.enabled(), Eq(false));
  EXPECT_THAT(default_toggle.effect()(0.0), DoubleEq(1.0));
  EXPECT_THAT(default_toggle(0.0), DoubleEq(0.0));
  EXPECT_THAT(default_toggle(0.1), DoubleEq(0.1));

  const auto const_toggle = MakeToggle([](Signal _) { return 1.5; }, false);
  const_toggle.enabled();
  const_toggle.effect();

  auto toggle = MakeToggle([](Signal _) { return 2.0; }, true);
  EXPECT_THAT(toggle.enabled(), Eq(true));
  EXPECT_THAT(toggle(0.0), DoubleEq(2.0));
  EXPECT_THAT(toggle(0.1), DoubleEq(2.0));

  EXPECT_THAT(toggle.enabled(false), Eq(false));
  EXPECT_THAT(toggle(0.0), DoubleEq(0.0));
  EXPECT_THAT(toggle(0.1), DoubleEq(0.1));
}

TEST(Toggle, CallFunctorEvenWhenDisabled) {
  testing::MockFunction<Signal(Signal signal)> functor;
  EXPECT_CALL(functor, Call(_)).WillOnce(Return(1.0));

  auto toggle = MakeToggle(functor.AsStdFunction(), false);
  EXPECT_THAT(toggle(0.0), DoubleEq(0.0));
}

}  // ymoch::pedalpieffects::dsp::effect::amplification
