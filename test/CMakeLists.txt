# test/CMakeLists.txt

# Create test executable for C
add_executable(test_c test_c.c)
target_include_directories(test_c PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
)
target_sources(test_c PRIVATE
    ${CMAKE_SOURCE_DIR}/src/logger_core.c
    ${CMAKE_SOURCE_DIR}/src/logger_linux.c
)
target_compile_definitions(test_c PRIVATE PLATFORM_LINUX)

# Create test executable for C++
add_executable(test_cpp test_cpp.cpp)
target_include_directories(test_cpp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
)
target_sources(test_cpp PRIVATE
    ${CMAKE_SOURCE_DIR}/src/logger_core.c
    ${CMAKE_SOURCE_DIR}/src/logger_linux.c
)
target_compile_definitions(test_cpp PRIVATE PLATFORM_LINUX)
set_target_properties(test_cpp PROPERTIES LINKER_LANGUAGE CXX)
