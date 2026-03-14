#pragma once

#include <enum.h>

namespace Helper {
namespace Logger {

/**
 * @enum LogLevel
 * @brief Defines severity levels for logging.
 */
BETTER_ENUM(logLevel, int, NONE = 0, INFO, ERROR, WARNING, DEBUG)

} // namespace Logger
} // namespace Helper
