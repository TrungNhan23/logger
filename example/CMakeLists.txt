file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/example/*.cpp
)

add_executable(main_logger ${EXAMPLE_SOURCES})

target_compile_options(main_logger PRIVATE -O0 -g --coverage)
target_link_options(main_logger PRIVATE --coverage)

target_link_libraries(main_logger
    PRIVATE
    logger_lib
)
