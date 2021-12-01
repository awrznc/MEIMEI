cmake_minimum_required(VERSION 3.10)
project(MEIMEI_Generator C)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/bitmap/)
add_executable(meimei_generator ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)
target_include_directories(meimei_generator PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include/)
target_link_libraries(meimei_generator bitmap)

install(TARGETS meimei_generator RUNTIME DESTINATION bin)

enable_testing()

add_test( NAME meimei_generator_no_options COMMAND $<TARGET_FILE:meimei_generator> )
set_tests_properties( meimei_generator_no_options PROPERTIES PASS_REGULAR_EXPRESSION "meimei_generator: Not enough arguments." )

add_test( NAME meimei_generator_help COMMAND $<TARGET_FILE:meimei_generator> "--help" )
set_tests_properties( meimei_generator_help PROPERTIES PASS_REGULAR_EXPRESSION "meimei_generator" "options" "path" )

add_test(
  NAME meimei_generator_binary
  COMMAND $<TARGET_FILE:meimei_generator>
  "--target" "binary"
  "--import" "${CMAKE_CURRENT_SOURCE_DIR}/share/320x200.bmp"
  "--export" "./ctest_generate_binary.h"
)
set_tests_properties( meimei_generator_binary PROPERTIES PASS_REGULAR_EXPRESSION "meimei_generator: Export header file." )

add_test(
  NAME meimei_generator_font
  COMMAND $<TARGET_FILE:meimei_generator>
  "--target" "font"
  "--import" "${CMAKE_CURRENT_SOURCE_DIR}/share/font.bmp"
  "--export" "${CMAKE_CURRENT_BINARY_DIR}/ctest_generate_font.h"
)
set_tests_properties( meimei_generator_font PROPERTIES PASS_REGULAR_EXPRESSION "meimei_generator: Export header file." )
