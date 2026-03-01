#pragma once

#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include "ILogBackend.h"
#include "LogFormatter.hpp"
#include "LogLevel.hpp"

namespace Helper
{
namespace Logger
{

/**
 * @class Logger
 * @brief Thread-safe singleton logger that outputs formatted logs to console.
 *
 * This logger provides:
 * - Log level filtering
 * - printf-style formatted logging
 * - Thread-safe console output
 *
 * Designed as a simple base logger suitable for Linux or desktop environments.
 * Not intended for ISR or hard real-time environments.
 */
class Logger
{
public:
    /**
      * @brief Returns the singleton instance of Logger.
      *
      * Thread-safe since C++11.
      *
      * @return Reference to the global Logger instance.
      */
    static Logger& getInstance();

    /**
      * @brief Sets the minimum log level.
      *
      * Messages below this level will be ignored.
      *
      * @param level The minimum severity level to log.
      */
    void setLevel(logLevel& level);


    void addBackend(const std::shared_ptr<ILogBackend>& backend);
    
    /**
      * @brief Adds a log backend to output logs to.
      *
      */
    template<typename... Args>
    void printMessage(logLevel level,
                              const std::string& file,
                              int line,
                              const std::string& message,
                              Args&&... args)
    {
      // if (level < m_level)
      // {
      //     return;
      // }

      // Format the message using LogFormatter
      auto formattedMessage = LogFormatter::getInstance().format(level, file, line, message);

      std::lock_guard<std::mutex> lock(m_logMutex);
      for (const auto& backend : m_logBackends)
      {
          backend->write(formattedMessage);
      }
    }
    
    
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

protected:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    Logger() = default;

private:
  /**
   * @brief The current log level threshold.
   */
  logLevel m_level{logLevel::LOG_INFO};

  /**
    * @brief Mutex to ensure thread-safe logging.
    */
  std::mutex m_logMutex;
  
  /**
    * @brief List of log backends to output logs to.
    *
    * Each backend implements the ILogBackend interface, allowing for flexible
    * log output (e.g., console, file, network).
    */
  std::vector<std::shared_ptr<ILogBackend>> m_logBackends;
};

} // namespace Logger
} // namespace Helper
