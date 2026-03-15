file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/example/*.cpp
)

add_executable(main_logger ${EXAMPLE_SOURCES})

target_link_libraries(main_logger
    PRIVATE
    logger_lib
)
