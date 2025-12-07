cmake_minimum_required(VERSION 3.10)
project(LoggerProject C CXX)

set(CMAKE_CXX_STANDARD 17)

# =========================================================
# Include paths
# =========================================================
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/config)
include_directories(${CMAKE_SOURCE_DIR}/platform)

# =========================================================
# Enable coverage BEFORE creating targets
# =========================================================
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
option(ENABLE_LINT "Enable linting with clang-tidy" ON)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# =========================================================
# Build GoogleTest from source (NOT from system!)
# =========================================================
add_subdirectory(third-party/googletest)

# =========================================================
# Add test folder AFTER coverage + gtest is ready
# =========================================================
add_subdirectory(test)

# =========================================================
# Lint config (unchanged)
# =========================================================
if(ENABLE_LINT)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

        file(GLOB_RECURSE ALL_SOURCE_FILES
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/include/*.h
        )

        add_custom_target(lint
            COMMAND ${CLANG_TIDY_EXE}
                -p ${CMAKE_BINARY_DIR}
                --use-color
                --quiet
                ${ALL_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy"
            VERBATIM
        )

    else()
        message(WARNING "clang-tidy not found, 'make lint' will be unavailable")
    endif()
endif()

# =========================================================
# Build logger library
# =========================================================
add_library(logger_lib
    src/logger_core.c
    src/logger_linux.c
    src/logger.cpp
    src/FileBackend.cpp
    src/ConsoleBackend.cpp
    src/LogFormatter.cpp
)

target_include_directories(logger_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_SOURCE_DIR}/platform
)

target_compile_definitions(logger_lib PUBLIC PLATFORM_LINUX)
