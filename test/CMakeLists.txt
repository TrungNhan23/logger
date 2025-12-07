enable_testing()

# Collect include dirs
set(LOGGER_INCLUDES
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
)

# ============================================================
# GoogleTest-based test (mock + unit test)
# ============================================================
add_executable(test_logger
    test_logger.cpp
)

target_include_directories(test_logger PRIVATE ${LOGGER_INCLUDES})
target_compile_definitions(test_logger PRIVATE PLATFORM_LINUX)

target_link_libraries(test_logger
    PRIVATE
        logger_lib
        gtest
        gmock
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_logger)


# ============================================================
# Other plain C / C++ tests
# ============================================================
file(GLOB PLAIN_TESTS
    "${CMAKE_SOURCE_DIR}/test/*.[cC]"
    "${CMAKE_SOURCE_DIR}/test/*.[cC][pP][pP]"
)

# Remove GoogleTest-based test
list(REMOVE_ITEM PLAIN_TESTS "${CMAKE_SOURCE_DIR}/test/test_logger.cpp")

foreach(test_file ${PLAIN_TESTS})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_include_directories(${test_name} PRIVATE ${LOGGER_INCLUDES})
    target_compile_definitions(${test_name} PRIVATE PLATFORM_LINUX)

    target_link_libraries(${test_name} PRIVATE logger_lib)
endforeach()
