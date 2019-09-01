cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17 -Wall)

include_directories(.)

add_executable(
  pedal-pi-effects
  ymoch/pedalpieffects/dsp/effect/biquad-filter.cc
  ymoch/pedalpieffects/effect.cc
  ymoch/pedalpieffects/input-equalization.cc
  ymoch/pedalpieffects/main.cc
)

target_link_libraries(pedal-pi-effects bcm2835)

install(TARGETS pedal-pi-effects DESTINATION bin)
