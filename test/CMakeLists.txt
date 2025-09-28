# test/CMakeLists.txt

enable_testing()
find_package(GTest REQUIRED)

set(LOGGER_SOURCES
    ${CMAKE_SOURCE_DIR}/src/logger_core.c
    ${CMAKE_SOURCE_DIR}/src/logger_linux.c
    ${CMAKE_SOURCE_DIR}/src/logger.cpp        
    ${CMAKE_SOURCE_DIR}/src/FileBackend.cpp   
    ${CMAKE_SOURCE_DIR}/src/LogFormatter.cpp   
    ${CMAKE_SOURCE_DIR}/src/ConsoleBackend.cpp
)

set(LOGGER_INCLUDES
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
)

set(TEST_TARGETS
    test_core
    test_cpp
    test_c
)


# --- add GoogleTest target ---
add_executable(test_logger ${CMAKE_SOURCE_DIR}/test/test_logger.cpp)
target_sources(test_logger PRIVATE ${LOGGER_SOURCES})
target_include_directories(test_logger PRIVATE ${LOGGER_INCLUDES})
target_compile_definitions(test_logger PRIVATE PLATFORM_LINUX)
target_link_libraries(test_logger PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_logger)

# --- add other plain tests ---
file(GLOB TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/test/*.[cC]"
    "${CMAKE_SOURCE_DIR}/test/*.[cC][pP][pP]"
)
# file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/test/*.[cC]" "${CMAKE_SOURCE_DIR}/test/*.[cC][pP][pP]")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/test/test_logger.cpp")

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_sources(${test_name} PRIVATE ${LOGGER_SOURCES})
    target_include_directories(${test_name} PRIVATE ${LOGGER_INCLUDES})
    target_compile_definitions(${test_name} PRIVATE PLATFORM_LINUX)
endforeach()
