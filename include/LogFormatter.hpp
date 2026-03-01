#pragma once

#include <string>
#include <array>


#include "LogLevel.hpp"

namespace Helper
{
namespace Logger
{

/**
 * @struct LogFormat
 * @brief Represents a fully formatted log record including metadata.
 *
 * This structure contains all contextual information required
 * to represent a log message after formatting.
 *
 * It is typically produced by a LogFormatter and consumed
 * by a log backend (e.g., console, file, network).
 */
struct LogFormat
{
    /**
     * @brief Timestamp of the log entry.
     *
     * Expected to be pre-formatted as a human-readable string,
     * e.g., "12:00:00.123".
     */
    std::string m_timestamp;

    /**
     * @brief String representation of the log severity level.
     *
     * Example values: "DEBUG", "INFO", "WARN", "ERROR".
     */
    std::string m_level;

    /**
     * @brief Source file where the log was generated.
     *
     * Typically provided using the __FILE__ macro.
     */
    std::string m_file;

    /**
     * @brief Line number in the source file.
     *
     * Typically provided using the __LINE__ macro.
     */
    uint32_t m_line;

    /**
     * @brief Final formatted log message content.
     *
     * Contains the user-provided message after
     * printf-style formatting (if applicable).
     */
    std::string m_message;
}; // may usde in future for structured logging or log backends

/**
 * @class LogFormatter
 * @brief Singleton class responsible for formatting log messages.
 *
 * This class formats log messages into a human-readable string
 * according to predefined formatting rules.
 *
 * Format rule:
 *   file line [optional time] [LEVEL] message
 *
 * Timestamp (HH:MM:SS) is included only for DEBUG and ERROR levels.
 *
 * This class does not manage output destinations.
 * It only handles formatting responsibility.
 */
class LogFormatter
{
public:
    /**
     * @brief Returns the singleton instance of LogFormatter.
     *
     * Thread-safe since C++11 due to local static initialization.
     *
     * @return Reference to the global LogFormatter instance.
     */
    static LogFormatter& getInstance();

    /**
     * @brief Formats a log message according to the formatting policy.
     *
     * Format structure:
     *   file line [optional timestamp] [LEVEL] message
     *
     * Timestamp (HH:MM:SS) is included only when:
     *   - level == LOG_LEVEL_DEBUG
     *   - level == LOG_LEVEL_ERROR
     *
     * @param level   Log severity level.
     * @param file    Source file name (typically from __FILE__ macro).
     * @param line    Source line number (typically from __LINE__ macro).
     * @param message User formatted message.
     *
     * @return Fully formatted log string.
     */
    std::string format(logLevel& level,
                       const std::string& file,
                       int line,
                       const std::string& message);

    /**
     * @brief Deleted copy constructor.
     *
     * Prevents copying of singleton instance.
     */
    LogFormatter(const LogFormatter&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     *
     * Prevents assignment of singleton instance.
     */
    LogFormatter& operator=(const LogFormatter&) = delete;

private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    LogFormatter() = default;

    /**
     * @brief Default destructor.
     */
    ~LogFormatter() = default;

private:
    /**
     * @brief Returns the current time formatted as HH:MM:SS.
     *
     * Does not include date information.
     *
     * @return Current time as string.
     */
    std::string getCurrentTime() const;

    /**
     * @brief Converts a log level to its string representation.
     *
     * @param level Log severity level.
     * @return C-string representation of the level.
     */
    const char* levelToString(logLevel& level) const;
};

} // namespace Logger
} // namespace Helper
