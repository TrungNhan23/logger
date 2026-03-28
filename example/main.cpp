#include "Log.hpp"
#include "ConsoleBackend.h"
#include "FileBackend.h"

// This is a simple example to demonstrate the usage of the Logger class.
// It logs messages with different log levels and shows how the log level filtering works.
void logMessagesWithLevelsVerbose()
{
  Helper::Logger::Logger::getInstance().setCurrentLevel(Helper::Logger::LogLevel::VERBOSE);

  LOG_INFO("Current log level is ", Helper::Logger::Logger::getInstance().getCurrentLevel(), " so this message should be logged");

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
}

void logMessagesWithLevelsInfo()
{
  Helper::Logger::Logger::getInstance().setCurrentLevel(Helper::Logger::LogLevel::INFO);

  LOG_INFO("Current log level is ", Helper::Logger::Logger::getInstance().getCurrentLevel(), " so this message should be logged");

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
}

// This function is to test the NONE log level, which should only log ERROR messages.
void logMessagesWithLevelsNone()
{
  Helper::Logger::Logger::getInstance().setCurrentLevel(Helper::Logger::LogLevel::NONE);
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
}

int main()
{
  Helper::Logger::Logger::getInstance().
                          addBackend(std::make_shared<Helper::Logger::ConsoleBackend>());
  Helper::Logger::Logger::getInstance().
                          addBackend(std::make_shared<Helper::Logger::FileBackend>("log.txt"));

  logMessagesWithLevelsVerbose();
  logMessagesWithLevelsInfo();
  logMessagesWithLevelsNone();
  
  return 0;
}
