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
    void setCurrentLevel(LogLevel level);

    /**
      * @brief get the current log level.
      *
      * Messages below this level will be ignored.
      *
      * @return The current log level.
      */
    LogLevel getCurrentLevel() const;

    /**
      * @brief Adds a log backend to output logs to.
      *
      * Each backend implements the ILogBackend interface, allowing for flexible
      * log output (e.g., console, file,...).
      *
      * @param backend Shared pointer to a log backend instance.
      */
    void addBackend(const std::shared_ptr<ILogBackend>& backend);
    
    /**
      * @brief Adds a log backend to output logs to.
      *
      */
    template<typename... Args>
    void printMessage(LogLevel level,
                              const std::string& file,
                              int line,
                              const std::string& message,
                              Args&&... args)
    {
      // For handle log level filtering, we can check the current log level before formatting the message.
      // Only log all when the verbose level is set, otherwise log only messages with level equal or higher than the current log level.
      // If the current log level is NONE, only log ERROR messages.
      if (m_level._to_integral() == LogLevel::NONE)
      {
          if (level._to_integral() != LogLevel::ERROR)
          {
              return;
          }
      }
      else if (level < m_level)
      {
          return;
      }

      // Format the message using LogFormatter
      auto formattedMessage = m_formatter->format(level, file, line, message, std::forward<Args>(args)...);

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
  LogLevel m_level{LogLevel::INFO};

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

  std::unique_ptr<LogFormatter> m_formatter{std::make_unique<LogFormatter>()};
};

} // namespace Logger
} // namespace Helper
