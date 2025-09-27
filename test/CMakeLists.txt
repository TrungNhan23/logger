# test/CMakeLists.txt

# Create test executable for C
# add_executable(test_c test_c.c)
# target_include_directories(test_c PRIVATE
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/config
#     ${CMAKE_SOURCE_DIR}/platform
# )
# target_sources(test_c PRIVATE
#     ${CMAKE_SOURCE_DIR}/src/logger_core.c
#     ${CMAKE_SOURCE_DIR}/src/logger_linux.c
# )
# target_compile_definitions(test_c PRIVATE PLATFORM_LINUX)

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
    ${CMAKE_SOURCE_DIR}/src/logger.cpp        
    ${CMAKE_SOURCE_DIR}/src/FileBackend.cpp   
    ${CMAKE_SOURCE_DIR}/src/LogFormatter.cpp   
    ${CMAKE_SOURCE_DIR}/src/ConsoleBackend.cpp   

)
target_compile_definitions(test_cpp PRIVATE PLATFORM_LINUX)
set_target_properties(test_cpp PROPERTIES LINKER_LANGUAGE CXX)


# add_executable(test_core test_core.cpp)
# target_link_libraries(test_core PRIVATE gtest gtest_main pthread)
# target_include_directories(test_core PRIVATE
#     ${CMAKE_SOURCE_DIR}/include
#     ${CMAKE_SOURCE_DIR}/config
#     ${CMAKE_SOURCE_DIR}/platform
# )
# target_sources(test_core PRIVATE
#     ${CMAKE_SOURCE_DIR}/src/logger_core.c
#     # ${CMAKE_SOURCE_DIR}/src/logger_linux.c
# )
# target_compile_definitions(test_core PRIVATE PLATFORM_LINUX)
# set_target_properties(test_core PROPERTIES LINKER_LANGUAGE CXX)
