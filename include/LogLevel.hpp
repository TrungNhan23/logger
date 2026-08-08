#pragma once // NOLINT(llvm-header-guard)
#pragma once // NOLINT(llvm-header-guard)

#include <enum.h>

namespace Helper
{
namespace Logger
{
// NOLINTBEGIN

/**
 * @enum LogLevel
 * @brief Defines severity levels for logging.
 */
// NOLINTBEGIN
BETTER_ENUM(LogLevel, int, NONE = 0, VERBOSE, DEBUG, INFO, WARNING, ERROR)

/**
 * @enum BackendType
 * @brief Defines types of log backends.
 */
BETTER_ENUM(BackendType, uint16_t, CONSOLE, FILE, NETWORK);
// NOLINTEND

} // namespace Logger
} // namespace Helper
