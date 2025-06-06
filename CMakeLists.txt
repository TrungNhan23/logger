cmake_minimum_required(VERSION 3.10)
project(Logger C CXX)

# Public include directories
include_directories(include config)

# Logger core source
set(LOGGER_SRC
    src/logger_core.c
)

# Platform-specific source
if(UNIX)
    list(APPEND LOGGER_SRC src/logger_linux.c)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Generic")
    list(APPEND LOGGER_SRC src/logger_mcu.c)
endif()

# Create a library target
add_library(logger STATIC ${LOGGER_SRC})
