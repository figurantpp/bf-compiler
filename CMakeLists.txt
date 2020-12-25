cmake_minimum_required(VERSION 3.17)
project(bfc C)

set(CMAKE_C_STANDARD 99)

add_library(external
        external/stream.c external/stream.h)

add_executable(bfc main.c bfc/bfc.c bfc/bfc.h bfc/backend.c)

target_link_libraries(bfc external)
