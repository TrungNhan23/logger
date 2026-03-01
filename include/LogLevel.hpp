#pragma once

namespace Helper {
namespace Logger {

/**
 * @enum LogLevel
 * @brief Defines severity levels for logging.
 */
enum class logLevel
{
  LOG_NONE = 0,
  LOG_INFO,
  LOG_ERROR,
  LOG_WARN,
  LOG_DEBUG,
};

} // namespace Logger
} // namespace Helper