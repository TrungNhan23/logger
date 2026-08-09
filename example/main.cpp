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

    LOG_INFO("Current log level is ", lg->getCurrentLevel(), " so this message should be logged");
    LOG_INFO("Current log level is ", lg->getCurrentLevel(), " so this message should be logged");

    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");

    for (auto i = 0; i < 10; ++i)
    {
        LOG_DEBUG("Debug message ", i, " times", " with more details");
        LOG_INFO("Info message ", i, " times", " with more details");
        LOG_WARNING("Warning message ", i, " times", " with more details");
        LOG_ERROR("Error message ", i, " times", " with more details");
    }
    for (auto i = 0; i < 10; ++i)
    {
        LOG_DEBUG("Debug message ", i, " times", " with more details");
        LOG_INFO("Info message ", i, " times", " with more details");
        LOG_WARNING("Warning message ", i, " times", " with more details");
        LOG_ERROR("Error message ", i, " times", " with more details");
    }
}

void logMessagesWithLevelsInfo()
{
    auto lg = Helper::Logger::defaultLogger();
    if (!lg) return;
    lg->setCurrentLevel(Helper::Logger::LogLevel::INFO);

    LOG_INFO("Current log level is ", lg->getCurrentLevel(), " so this message should be logged");
    LOG_INFO("Current log level is ", lg->getCurrentLevel(), " so this message should be logged");

    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");

    for (auto i = 0; i < 10; ++i)
    {
        LOG_DEBUG("Debug message ", i, " times", " with more details");
        LOG_INFO("Info message ", i, " times", " with more details");
        LOG_WARNING("Warning message ", i, " times", " with more details");
        LOG_ERROR("Error message ", i, " times", " with more details");
    }
    for (auto i = 0; i < 10; ++i)
    {
        LOG_DEBUG("Debug message ", i, " times", " with more details");
        LOG_INFO("Info message ", i, " times", " with more details");
        LOG_WARNING("Warning message ", i, " times", " with more details");
        LOG_ERROR("Error message ", i, " times", " with more details");
    }
}

// This function is to test the NONE log level, which should only log ERROR
// messages.
void logMessagesWithLevelsNone()
{
    auto lg = Helper::Logger::defaultLogger();
    if (!lg) return;
    lg->setCurrentLevel(Helper::Logger::LogLevel::NONE);
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");
    lg->setCurrentLevel(Helper::Logger::LogLevel::NONE);
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARNING("This is a warning message");
    LOG_ERROR("This is an error message");

    for (auto i = 0; i < 10; ++i)
    {
        LOG_DEBUG("Debug message ", i, " times", " with more details");
        LOG_INFO("Info message ", i, " times", " with more details");
        LOG_WARNING("Warning message ", i, " times", " with more details");
        LOG_ERROR("Error message ", i, " times", " with more details");
    }
    for (auto i = 0; i < 10; ++i)
    {
        LOG_DEBUG("Debug message ", i, " times", " with more details");
        LOG_INFO("Info message ", i, " times", " with more details");
        LOG_WARNING("Warning message ", i, " times", " with more details");
        LOG_ERROR("Error message ", i, " times", " with more details");
    }
}

void testWithMultiThreading()
{
    constexpr int THREADS = 8;

    std::vector<std::thread> workers;

    for (int t = 0; t < THREADS; ++t)
    {
        workers.emplace_back(
            []()
            {
                for (int i = 0; i < 100000; ++i)
                {
                    LOG_INFO("hello {}", i);
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

    for (int i = 0; i < 100000; ++i)
    {
        LOG_DEBUG("hello {}", i);
        // auto s = fmt::format("hello {}", i);
    }

    auto end = std::chrono::steady_clock::now();

    LOG_DEBUG("Time taken with logging a message: {} microseconds", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
}

int main()
{
    auto logger_ptr = std::make_shared<Helper::Logger::Logger>("/home/user/logger/example/config/LogConfig.yaml");
    Helper::Logger::setDefaultLogger(logger_ptr);
    // Add a console backend so example logs are visible.
    if (auto lg = Helper::Logger::defaultLogger())
    {
        lg->addBackend(std::make_shared<Helper::Logger::ConsoleBackend>());
        LOG_INFO("Example logger initialized");
    }

/*     logMessagesWithLevelsVerbose();
    logMessagesWithLevelsInfo();
    logMessagesWithLevelsNone();

    testWithTimeMesurement();
 */
    return 0;
}
