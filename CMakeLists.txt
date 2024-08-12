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

set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost1.58.0 (>= 1.58)")
set(CPACK_PACKAGE_VENDOR "Khromenok Roman")
set(CPACK_PACKAGE_DESCRIPTION "Запускает радио удаленно")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "roma55592@yandex.ru")
include(CPack)