cmake_minimum_required(VERSION 3.15)
project(bfc C)

set(CMAKE_C_STANDARD 99)

add_library(external
        STATIC
        source/external/stream.c source/external/stream.h)

add_library(bfc_core
        STATIC
        source/core/frontend.c source/core/frontend.h source/core/backend.c
        source/core/backend.h)

add_library(bfc_compilation
        STATIC
        source/compilation/compilation.c source/compilation/compilation.h)

add_library(asm_format
        source/asm_format/asm_format.h source/asm_format/gnu_asm_format.c source/asm_format/yasm_asm_format.c)

add_library(user_interface source/user_interface/user_interface.c source/user_interface/user_interface.h source/user_interface/user_interface_static.h)

add_executable(bfc main.c)

target_link_libraries(bfc user_interface)
target_link_libraries(bfc bfc_compilation)
target_link_libraries(bfc external)
target_link_libraries(bfc bfc_core)
target_link_libraries(bfc asm_format)
