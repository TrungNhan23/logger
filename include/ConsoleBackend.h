#pragma once

#include <string>

#include "ILogBackend.h"

namespace Helper
{
namespace Logger
{

/**
 * @brief Log backend that writes messages to the console.
 *
 * ConsoleBackend is a concrete implementation of ILogBackend that outputs
 * formatted log messages to the standard console stream (typically stdout).
 *
 * This backend is commonly used during development and debugging to
 * immediately observe log messages.
 *
 * @note The message provided to write() is expected to be already formatted
 *       by the logger or formatter component.
 */
class ConsoleBackend : public ILogBackend
{
public:

  /**
   * @brief Constructs a ConsoleBackend instance.
   *
   * Initializes a console logging backend that outputs log messages
   * to the standard output stream.
   */
  ConsoleBackend() = default;
  ~ConsoleBackend() override = default;

  ConsoleBackend(const ConsoleBackend&) = default;
  ConsoleBackend& operator=(const ConsoleBackend&) = default;

  ConsoleBackend(ConsoleBackend&&) = default;
  ConsoleBackend& operator=(ConsoleBackend&&) = default;

  /**
   * @brief Destructor.
   *
   * Performs cleanup for the console backend.
   * No explicit resource management is required.
   */
  // ~ConsoleBackend() override = default;

  /**
   * @brief Writes a formatted log message to the console.
   *
   * Sends the provided log message to the console output stream.
   *
   * @param formattedMsg The fully formatted log message.
   */
  void write(const std::string &formattedMsg) override;
};
} // namespace Logger
} // namespace Helper
