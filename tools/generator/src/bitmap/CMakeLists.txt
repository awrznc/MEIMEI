cmake_minimum_required(VERSION 3.10)
project(libbitmap C)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_library(bitmap STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/bitmap.c)
target_include_directories(bitmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# test
enable_testing()
add_executable(bitmap_test ${CMAKE_CURRENT_SOURCE_DIR}/test/test_main.c ${CMAKE_CURRENT_SOURCE_DIR}/src/bitmap.c)
target_include_directories(bitmap_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set_property(TARGET bitmap_test PROPERTY C_STANDARD 11)
add_test( NAME "bitmap_test" COMMAND $<TARGET_FILE:bitmap_test> )
