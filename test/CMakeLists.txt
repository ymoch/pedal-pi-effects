cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17 -Wall)

include_directories(
  ../src
)

add_executable(
  pedal-pi-effects-test
  ymoch/pedalpieffects/math/constexpr-math-test.cc
)

target_link_libraries(
  pedal-pi-effects-test
  gmock_main
  gmock
  gtest
)
