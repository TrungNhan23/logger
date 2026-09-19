#pragma once // NOLINT(llvm-header-guard)

#include <functional>
#include <memory>

#include "Logger.hpp"

namespace Helper::Logger
{
// Register a factory that will be used to lazily create the default logger.
// The factory should return a shared_ptr to a Logger instance.
void setLoggerFactory(std::function<std::shared_ptr<Logger>()> factory);

// Optionally set the default logger instance directly.
void setDefaultLogger(std::shared_ptr<Logger> logger);

// Return the default logger. If a logger has not been created yet, the
// registered factory is used to create it lazily. Returns nullptr if no
// factory or logger is available.
std::shared_ptr<Logger> defaultLogger();

} // namespace Helper::Logger
