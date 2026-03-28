#pragma once

#include <enum.h>

namespace Helper {
namespace Logger {

/**
 * @enum LogLevel
 * @brief Defines severity levels for logging.
 */
BETTER_ENUM(LogLevel, int, NONE = 0, VERBOSE, DEBUG, INFO, WARNING, ERROR)

} // namespace Logger
} // namespace Helper
