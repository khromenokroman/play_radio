set(APP_NAME radio_server)
message(STATUS "Добавлен в сборку ${APP_NAME}")

find_package(PkgConfig REQUIRED)
find_package(fmt)

set(PLUG_SOURCES
        server.cpp
        main.cpp
)
add_executable(${APP_NAME} ${PLUG_SOURCES})
install(TARGETS ${APP_NAME} DESTINATION bin)

target_link_libraries(${APP_NAME} fmt::fmt)