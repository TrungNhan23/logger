#pragma once // NOLINT(llvm-header-guard)

#include <enum.h>

namespace Helper
{
namespace Logger
{

/**
 * @enum LogLevel
 * @brief Defines severity levels for logging.
 */
// NOLINTBEGIN
BETTER_ENUM(LogLevel, int, NONE = 0, VERBOSE, DEBUG, INFO, WARNING, ERROR)
// NOLINTEND

} // namespace Logger
} // namespace Helper
