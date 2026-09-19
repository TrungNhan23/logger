#include <thread>
#include <vector>
#include <iostream>

#include "ConsoleBackend.hpp"
#include "FileBackend.hpp"
#include "LogProvider.hpp"
#include "Log.hpp"

// This is a simple example to demonstrate the usage of the Logger class.
// It logs messages with different log levels and shows how the log level
// filtering works.
void logMessagesWithLevelsVerbose()
{
    auto lg = Helper::Logger::defaultLogger();
    if (!lg) return;
    lg->setCurrentLevel(Helper::Logger::LogLevel::VERBOSE);

    LOG_INFO("Current log level is {} so this message should be logged", lg->getCurrentLevel()._to_string());

    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");

    for (auto i = 0; i < 5; ++i)
    {
        LOG_DEBUG("Debug message {} times with more details", i);
        LOG_INFO("Info message {} times with more details", i);
        LOG_WARNING("Warning message {} times with more details", i);
        LOG_ERROR("Error message {} times with more details", i);
    }
}

void logMessagesWithLevelsInfo()
{
    auto lg = Helper::Logger::defaultLogger();
    if (!lg) return;
    lg->setCurrentLevel(Helper::Logger::LogLevel::INFO);

    LOG_INFO("Current log level is {} so this message should be logged", lg->getCurrentLevel()._to_string());

    LOG_DEBUG("This is a debug message (should not be logged in INFO mode)");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
}

// This function is to test the NONE log level, which should only log ERROR messages.
void logMessagesWithLevelsNone()
{
    auto lg = Helper::Logger::defaultLogger();
    if (!lg) return;
    lg->setCurrentLevel(Helper::Logger::LogLevel::NONE);

    LOG_DEBUG("This debug message should not be logged");
    LOG_INFO("This info message should not be logged");
    LOG_WARNING("This warning message should not be logged");
    LOG_ERROR("This error message SHOULD be logged");
}

void testWithMultiThreading()
{
    constexpr int THREADS = 4;

    std::vector<std::thread> workers;

    for (int t = 0; t < THREADS; ++t)
    {
        workers.emplace_back(
            []()
            {
                for (int i = 0; i < 100; ++i)
                {
                    LOG_INFO("Thread worker message: {}", i);
                }
            });
    }

    for (auto& worker : workers)
    {
        worker.join();
    }
}

void testWithTimeMesurement()
{
    auto lg = Helper::Logger::defaultLogger();
    if (!lg) return;
    lg->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < 1000; ++i)
    {
        LOG_DEBUG("Benchmark message {}", i);
    }

    auto end = std::chrono::steady_clock::now();

    LOG_DEBUG("Time taken for 1000 messages: {} microseconds", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

int main(int argc, char* argv[])
{
    std::string configPath = (argc > 1) ? argv[1] : "example/config/LogConfig.yaml";
    auto logger_ptr = std::make_shared<Helper::Logger::Logger>(configPath);
    Helper::Logger::setDefaultLogger(logger_ptr);

    LOG_INFO("Example logger initialized from config: {}", configPath);

    logMessagesWithLevelsVerbose();
    logMessagesWithLevelsInfo();
    logMessagesWithLevelsNone();
    testWithMultiThreading();
    testWithTimeMesurement();

    return 0;
}
