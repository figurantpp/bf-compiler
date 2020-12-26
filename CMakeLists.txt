cmake_minimum_required(VERSION 3.17)
project(bfc C)

set(CMAKE_C_STANDARD 99)

add_library(external
        STATIC
        external/stream.c external/stream.h)

add_library(bfc_core
        STATIC
        core/frontend.c core/frontend.h core/backend.c
        core/frontend_asm.h)

add_library(bfc_compilation
        STATIC

        compilation/compilation.c compilation/compilation.h)

add_executable(bfc main.c)

target_link_libraries(bfc bfc_compilation)
target_link_libraries(bfc external)
target_link_libraries(bfc bfc_core)
