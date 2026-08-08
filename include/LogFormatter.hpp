#ifndef LOGGER_INCLUDE_LOGFORMATTER_HPP // NOLINT(llvm-header-guard)
#define LOGGER_INCLUDE_LOGFORMATTER_HPP
#ifndef LOGGER_INCLUDE_LOGFORMATTER_HPP // NOLINT(llvm-header-guard)
#define LOGGER_INCLUDE_LOGFORMATTER_HPP

#include <array>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <optional>

#include <fmt/format.h>

#include "LogLevel.hpp"

namespace Helper
{
namespace Logger
{

constexpr size_t SIZE_OF_BUFFER = 1024; // Buffer size for formatted messages

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
    template<typename... Args>
    std::string format(LogLevel level,/*  const std::string& file, int line,  */const std::string& message, Args&&... args)
    {
        std::ostringstream oss;

        if (level._to_integral() == LogLevel::DEBUG || level._to_integral() == LogLevel::ERROR)
        {
            oss << getCurrentTime() << " ";
        }

        std::string formatted = fmt::format(message, std::forward<Args>(args)...);
        oss << "[" << level._to_string() << "] " << formatted;
        std::string formatted = fmt::format(message, std::forward<Args>(args)...);
        oss << "[" << level._to_string() << "] " << formatted;

        return oss.str();
    }

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
    LogFormatter(LogFormatter&&) = delete;
    LogFormatter& operator=(LogFormatter&&) = delete;
    LogFormatter(LogFormatter&&) = delete;
    LogFormatter& operator=(LogFormatter&&) = delete;

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
    static std::string getCurrentTime();
    static std::string getCurrentTime();
};

} // namespace Logger
} // namespace Helper
#endif // LOGGER_INCLUDE_LOGFORMATTER_HPP
#endif // LOGGER_INCLUDE_LOGFORMATTER_HPP
