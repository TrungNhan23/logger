add_executable(benchmark_logger benchmark_logger.cpp)

target_link_libraries(benchmark_logger
    PRIVATE
    logger_lib
    pthread
)

target_compile_options(benchmark_logger PRIVATE -O3)

if(ENABLE_COVERAGE)
    target_link_options(benchmark_logger PRIVATE --coverage)
endif()

