#include "Log.hpp"
#include "ConsoleBackend.h"

int main()
{

  auto consoleBackend = std::make_shared<ConsoleBackend>(); 
  Helper::Logger::Logger::getInstance().addBackend(consoleBackend);


  LOG_DEBUG("This is a debug message");
  LOG_INFO("This is an info message");
  LOG_WARN("This is a warning message");
  LOG_ERROR("This is an error message");

  for (auto i = 0; i < 10; ++i)
  {
    LOG_DEBUG("Debug message %d", i); // wring behavior: should not right value
                                      // but should write "Debug message %d"
    LOG_INFO("Info message %d", i);
    LOG_WARN("Warning message %d", i);
    LOG_ERROR("Error message %d", i);
  }
}