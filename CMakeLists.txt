cmake_minimum_required(VERSION 3.10)
project(LoggerProject)

# Add include dir globally
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/config)
include_directories(${CMAKE_SOURCE_DIR}/platform)

# Add test
add_subdirectory(test)

option(ENABLE_LINT "Enable linting with clang-tidy" ON)

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
            COMMENT "Running clang-tidy with AUTOSAR-like checks..."
            VERBATIM
        )

    else()
        message(WARNING "clang-tidy not found, 'make lint' will be unavailable")
    endif()
endif()