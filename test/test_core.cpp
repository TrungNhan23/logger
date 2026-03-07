#include "Log.hpp"
#include "ConsoleBackend.h"

int main()
{
  Helper::Logger::Logger::getInstance().
                          addBackend(std::make_shared<Helper::Logger::ConsoleBackend>());


  LOG_DEBUG("This is a debug message");
  LOG_INFO("This is an info message");
  LOG_WARN("This is a warning message");
  LOG_ERROR("This is an error message");

  for (auto i = 0; i < 10; ++i)
  {
    LOG_DEBUG("Debug message ", i, " times", " with more details");
    LOG_INFO("Info message ", i, " times", " with more details");
    LOG_WARN("Warning message ", i, " times", " with more details");
    LOG_ERROR("Error message ", i, " times", " with more details");
  }
}