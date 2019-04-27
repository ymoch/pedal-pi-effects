#include "ymoch/pedalpieffects/dsp/effect/tube-clipping.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::DoubleEq;

namespace ymoch::pedalpieffects::dsp::effect::tube_clipping {

TEST(TubeClipper, ClipsSignal) {
  const TubeClipper clipper;

  EXPECT_THAT(clipper(-10000.0), DoubleEq(-0.8));
  EXPECT_THAT(clipper(0.0), DoubleEq(0.0));
  EXPECT_THAT(clipper(10000.0), DoubleEq(1.0));
}

}
