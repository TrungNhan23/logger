cmake_minimum_required(VERSION 3.10)
project(LoggerProject)

# Add include dir globally
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/config)
include_directories(${CMAKE_SOURCE_DIR}/platform)

# Add test
add_subdirectory(test)