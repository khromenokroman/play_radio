cmake_minimum_required(VERSION 3.25)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
project(play_radio VERSION ${PROJECT_VESRION})

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
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost1.58.0 (>= 1.58)")
set(CPACK_PACKAGE_VENDOR "Khromenok Roman")
set(CPACK_PACKAGE_DESCRIPTION "Запускает радио удаленно")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "roma55592@yandex.ru")
install(FILES radio_list DESTINATION /etc/radio)
install(FILES play_radio.service DESTINATION /var/lib/systemd/system)
include(CPack)