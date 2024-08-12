find_package(GTest REQUIRED)

include(GoogleTest)

set(TEST play_radio_test)
find_package(fmt CONFIG REQUIRED)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-format-truncation
        -Wnon-virtual-dtor
        -Wno-stringop-overflow
        -Wno-stringop-truncation
        -fprofile-arcs
        -ftest-coverage
        -fprofile-arcs
)
add_link_options(
        -Wno-undefined
        -lgcov
)
enable_testing()

add_definitions(-DPATH="${CMAKE_CURRENT_BINARY_DIR}")

add_executable(${TEST}
        tests.cpp
        ../src/client/client.cpp
        ../src/server/server.cpp
)

target_include_directories(${TEST} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${DBUS_INCLUDE_DIRS})

target_link_libraries(${TEST} PRIVATE GTest::gtest_main gcov fmt::fmt-header-only)

gtest_discover_tests(${TEST})

set(TESTING play_radio_testing)

add_custom_target(
        ${TESTING}
        ${TEST}
        --gtest_output=xml:${REPORT_DIR}/
)