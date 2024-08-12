set(APP_NAME radio)

execute_process(
        COMMAND bash -c "grep ^ID= /etc/os-release | cut -d'=' -f2"
        OUTPUT_VARIABLE LINUX_DISTRIBUTION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Название ОС: ${LINUX_DISTRIBUTION}")

find_package(PkgConfig)

add_subdirectory(client)
add_subdirectory(server)