cmake_minimum_required(VERSION 3.10)
project(meimei_project ASM C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ipl
add_executable(ipl ${CMAKE_CURRENT_SOURCE_DIR}/src/boot_loader/ipl/ipl.s)
target_link_options(ipl PRIVATE LINKER:
  -nostdinc
  -nostdlib
  -T${CMAKE_CURRENT_SOURCE_DIR}/src/boot_loader/ipl/ipl.ld
)
set_target_properties(ipl PROPERTIES OUTPUT_NAME ipl.bin)

# boot
add_executable(boot ${CMAKE_CURRENT_SOURCE_DIR}/src/boot_loader/boot/boot.s)
target_link_options(boot PRIVATE LINKER:
  -nostdinc
  -nostdlib
  -T${CMAKE_CURRENT_SOURCE_DIR}/src/boot_loader/boot/boot.ld
)
set_target_properties(boot PROPERTIES OUTPUT_NAME boot.bin)

# Generate font
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/font.h
  COMMAND meimei_generator
  ARGS --target font --import ${CMAKE_CURRENT_SOURCE_DIR}/tools/generator/share/font.bmp --export ${CMAKE_CURRENT_BINARY_DIR}/font.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tools/generator/share/font.bmp
)

# kernel
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/meimei/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libc/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/device/
)
add_executable(
  kernel

  # kernel
  ${CMAKE_CURRENT_BINARY_DIR}/font.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/kernel.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/graphic.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/queue.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/device/bios.s

  # libc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libc/pure/stdio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libc/pure/string.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libc/stdio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/libc/stdlib.c

  # application
  ${CMAKE_CURRENT_SOURCE_DIR}/src/meimei/meimei.c
)
target_compile_options(kernel PRIVATE
  -std=c99
  -m32
  -fno-pie
  -fno-builtin

  # error
  -Wall
  -Wextra
  -Waggregate-return
)
target_link_options(kernel PRIVATE LINKER:
  -nostdinc
  -nostdlib
  -e kernel_main
  -Wl,-melf_i386
  -Wl,--oformat=binary
  -T${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/kernel.ld
)
set_target_properties(kernel PROPERTIES OUTPUT_NAME kernel.bin)

# Generate OS Image
add_custom_target(format ALL
  COMMAND cat ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/boot.bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/kernel.bin > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meimei.sys
  COMMAND mformat -f 1440 -B ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ipl.bin -C -i ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meimei.img ::
  COMMAND mcopy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meimei.sys -i ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meimei.img ::
  COMMAND cp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meimei.img ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/meimei.iso
  DEPENDS ipl boot kernel
)
