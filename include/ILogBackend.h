#pragma once

#include <string>

namespace Helper
{
namespace Logger
{

/**
 * @brief Interface for log output backends.
 *
 * ILogBackend defines the abstraction for all logging output targets
 * (also called sinks or backends). A backend is responsible for delivering
 * a formatted log message to a specific destination such as:
 *
 * - Console (stdout / stderr)
 * - File
 * - Hardware interface (UART, CAN, etc.)
 * - Network socket
 * - Custom protocol
 *
 * The logger core generates a formatted message and forwards it to all
 * registered backends through the write() function.
 *
 * Implementations must provide the write() method to handle how the
 * message is delivered to the underlying output target.
 *
 * @note The message passed to write() is already formatted by the logger.
 *       Backend implementations should not modify the content.
 */
class ILogBackend
{
public:

  /**
   * @brief Default constructor.
   */
  ILogBackend() = default;

  /**
   * @brief Copy constructor.
   */
  ILogBackend(const ILogBackend &) = default;

  /**
   * @brief Copy assignment operator.
   */
  auto operator=(const ILogBackend &) -> ILogBackend & = default;

  /**
   * @brief Move constructor.
   */
  ILogBackend(ILogBackend &&) = default;

  /**
   * @brief Move assignment operator.
   */
  auto operator=(ILogBackend &&) -> ILogBackend & = default;

  /**
   * @brief Virtual destructor.
   *
   * Ensures proper cleanup of derived backend implementations when
   * deleted through a base class pointer.
   */
  virtual ~ILogBackend() = default;

  /**
   * @brief Writes a formatted log message to the backend destination.
   *
   * This function is called by the logger core to deliver a log message
   * that has already been formatted.
   *
   * @param formattedMsg Fully formatted log message ready for output.
   *
   * @note Implementations should ensure this operation is efficient
   *       and thread-safe if the logger supports multi-threading.
   */
  virtual void write(const std::string &formattedMsg) = 0;
};

} // namespace Logger
} // namespace Helper
