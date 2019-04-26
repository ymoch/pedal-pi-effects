cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17 -Wall)

add_executable(
  pedal-pi-effects
  src/ymoch/pedalpieffects/main.cc
)

target_link_libraries(pedal-pi-effects bcm2835)

install(
  TARGETS pedal-pi-effects
  DESTINATION bin
)
