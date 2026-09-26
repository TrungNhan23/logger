cmake_minimum_required(VERSION 3.16)
project(LoggerProject C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
find_package(fmt REQUIRED)

# =========================================================
# Enable coverage BEFORE creating targets
# =========================================================
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
option(ENABLE_LINT "Enable linting with clang-tidy" ON)

# =========================================================
# Build logger library
# =========================================================
file(GLOB LOGGER_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# =========================================================
# Header-only dependency
# =========================================================
add_library(better_enums INTERFACE)

target_include_directories(better_enums SYSTEM INTERFACE
    ${CMAKE_SOURCE_DIR}/third-party/better-enums
)

# =========================================================
# Logger library target
# =========================================================
add_library(logger_lib ${LOGGER_SOURCES})

target_link_libraries(
    logger_lib
    PUBLIC
    fmt::fmt
    better_enums
)

# Link platform-specific libraries
if(UNIX)
    target_link_libraries(logger_lib PUBLIC rt dl)
endif()

target_include_directories(logger_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    # ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(better_enums SYSTEM INTERFACE
    ${CMAKE_SOURCE_DIR}/third-party/better-enums
)

target_compile_definitions(logger_lib PUBLIC PLATFORM_LINUX)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    target_compile_options(logger_lib PRIVATE -O0 -g --coverage)
    target_link_options(logger_lib PRIVATE --coverage)
endif()

# =========================================================
# Build GoogleTest from source (NOT from system!)
# =========================================================
add_subdirectory(third-party/googletest)

# =========================================================
# Example executable
# =========================================================
add_subdirectory(example)

# =========================================================
# Add test folder AFTER coverage + gtest is ready
# =========================================================
add_subdirectory(test)

# =========================================================
# Benchmark executable
# =========================================================
add_subdirectory(bench)

# =========================================================
# Lint config
# =========================================================
if(ENABLE_LINT)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)   
    if(CLANG_TIDY_EXE)

        add_custom_target(lint
            COMMENT "Running clang-tidy"
        )

        file(GLOB_RECURSE ALL_SOURCE_FILES
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/include/*.hpp
        )

        list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*/third-party/.*")

        set(LINT_COMMANDS)

        foreach(FILE ${ALL_SOURCE_FILES})
            add_custom_command(
                TARGET lint
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Checked ${FILE}"
                COMMAND ${CLANG_TIDY_EXE}
                    -p ${CMAKE_BINARY_DIR}
                    --quiet
                    "--header-filter=^${CMAKE_SOURCE_DIR}/(include|src)/"
                    ${FILE}
                VERBATIM
            )
        endforeach()
    else()
        message(WARNING "clang-tidy not found, 'make lint' will be unavailable")
    endif()
endif()
