enable_testing()

# ============================================================
# GoogleTest-based test (mock + unit test)
# ============================================================
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/test/*.cpp
)

add_executable(test_logger ${TEST_SOURCES})

target_link_libraries(test_logger
    PRIVATE
    logger_lib
    gtest
    gmock
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_logger)

target_compile_options(test_logger PRIVATE -O0 -g --coverage)
target_link_options(test_logger PRIVATE --coverage)
