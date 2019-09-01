cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17 -Wall)
find_package(Threads REQUIRED)

include_directories(
  ../src
)

add_executable(
  pedal-pi-effects-test
  ../src/ymoch/pedalpieffects/dsp/effect/biquad-filter.cc
  ymoch/pedalpieffects/dsp/normalization-test.cc
  ymoch/pedalpieffects/dsp/oscillation-test.cc
  ymoch/pedalpieffects/dsp/effect/amplification-test.cc
  ymoch/pedalpieffects/dsp/effect/biquad-filter-test.cc
  ymoch/pedalpieffects/dsp/effect/tube-clipping-test.cc
  ymoch/pedalpieffects/dsp/flow/chain-test.cc
  ymoch/pedalpieffects/dsp/flow/split-test.cc
  ymoch/pedalpieffects/dsp/flow/toggle-test.cc
  ymoch/pedalpieffects/math/constexpr-math-test.cc
)

target_link_libraries(
  pedal-pi-effects-test
  gmock_main
  gmock
  gtest
  Threads::Threads
)
