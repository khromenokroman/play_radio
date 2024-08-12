cmake_minimum_required(VERSION 3.25)

project(play_radio)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-format-truncation
        -Wnon-virtual-dtor
        -Wno-stringop-overflow
        -Wno-stringop-truncation
)

add_link_options(
        -Wno-undefined
)

add_subdirectory(src)
add_subdirectory(tests)