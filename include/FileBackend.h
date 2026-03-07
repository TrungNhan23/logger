#pragma once

#include <fstream>
#include <string>

#include "ILogBackend.h"

namespace Helper
{
namespace Logger
{

/**
 * @brief Log backend that writes log messages to a file.
 *
 * FileBackend is a concrete implementation of ILogBackend that sends
 * formatted log messages to a file on the filesystem.
 *
 * The file is opened during construction and subsequent log messages
 * are appended through the write() method.
 *
 * @note The message passed to write() is expected to be already formatted
 *       by the logger or formatter component.
 */
class FileBackend : public ILogBackend
{
public:

  /**
   * @brief Constructs a FileBackend instance.
   *
   * Opens the specified file and prepares it for logging output.
   *
   * @param fileName Path of the file where log messages will be written.
   *
   * @note Typically the file is opened in append mode so existing
   *       log data is preserved.
   */
  explicit FileBackend(const std::string &fileName);

  /**
   * @brief Destructor.
   *
   * Ensures the file stream is properly closed when the backend
   * instance is destroyed.
   *
   * @note std::ofstream automatically closes the file when destroyed,
   *       so no explicit cleanup is typically required.
   */
  // ~FileBackend() override = default;

  /**
   * @brief Writes a formatted log message to the file.
   *
   * This function appends the provided log message to the opened file.
   *
   * @param formattedMessage The fully formatted log message.
   */
  void write(const std::string &formattedMessage) override;

private:

  /**
   * @brief Output file stream used for writing log messages.
   */
  std::ofstream m_fileName;
};

} // namespace Logger
} // namespace Helper
