#pragma once

#include <string>

#include "logger.h"

class LogFormatter
{
public:
  static auto format(logLevel level, const std::string message) -> std::string;
};
