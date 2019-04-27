cmake_minimum_required(VERSION 3.7)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(
  NAME pedal-pi-effects-test
  COMMAND ./pedal-pi-effects-test
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(
  TARGETS pedal-pi-effects
  DESTINATION bin
)
