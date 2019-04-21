cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)
add_executable(
  pedal-pi-effects
  src/ymoch/pedalpieffects/main.cc
)

install(
  TARGETS pedal-pi-effects
  DESTINATION bin
)
