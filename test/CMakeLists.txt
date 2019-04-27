cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17 -Wall)

add_executable(
  pedal-pi-effects-test
  main.cc
)

target_link_libraries(
  pedal-pi-effects-test
  gmock_main
  gmock
  gtest
)
