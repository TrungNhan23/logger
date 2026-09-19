#pragma once // NOLINT(llvm-header-guard)

#include <array>
#include <chrono>
#include <ctime>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include "LogFormatterConfigParser.hpp"
#include "LogLevel.hpp"

namespace Helper::Logger
{

constexpr size_t SIZE_OF_BUFFER = 1024; // Buffer size for formatted messages

/**
 * @class LogFormatter
 * @brief Class responsible for formatting log messages.
 *
 * This class formats log messages into a human-readable string
 * according to predefined formatting rules or parsed YAML configuration.
 */
class LogFormatter
{
public:
    /**
     * @brief Formats a log message according to the formatting policy or configuration.
     *
     * @param level   Log severity level.
     * @param message Format string.
     * @param args    Formatting arguments.
     * @return Fully formatted log string.
     */
    template<typename... Args>
    std::string format(LogLevel level, const std::string& message, Args&&... args)
    {
        std::string formatted = fmt::format(message, std::forward<Args>(args)...);

        if (m_configParser)
        {
            const auto& fields = m_configParser->getFields();
            std::string separator = fields.m_separator.value_or(" ");
            std::vector<std::string> parts;

            if (fields.m_timestamp)
            {
                parts.push_back(getCurrentTime());
            }
            if (fields.m_level)
            {
                parts.push_back(std::string("[") + level._to_string() + "]");
            }
            if (fields.m_moduleName)
            {
                parts.push_back(m_moduleName);
            }
            if (fields.m_threadId)
            {
                std::ostringstream tidStream;
                tidStream << std::this_thread::get_id();
                parts.push_back(tidStream.str());
            }
            parts.push_back(formatted);

            std::ostringstream oss;
            for (size_t i = 0; i < parts.size(); ++i)
            {
                if (i > 0)
                {
                    oss << separator;
                }
                oss << parts[i];
            }
            return oss.str();
        }

        std::ostringstream oss;
        if (level._to_integral() == LogLevel::DEBUG || level._to_integral() == LogLevel::ERROR)
        {
            oss << getCurrentTime() << " ";
        }
        oss << "[" << level._to_string() << "] " << formatted;

        return oss.str();
    }

    /**
     * @brief Sets the module name.
     *
     * @param moduleName Module name string.
     */
    void setModuleName(std::string moduleName)
    {
        m_moduleName = std::move(moduleName);
    }

    /**
     * @brief Gets the current module name.
     *
     * @return Module name string.
     */
    [[nodiscard]] const std::string& getModuleName() const
    {
        return m_moduleName;
    }

    /**
     * @brief Deleted copy constructor.
     */
    LogFormatter(const LogFormatter&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    LogFormatter& operator=(const LogFormatter&) = delete;
    LogFormatter(LogFormatter&&) = delete;
    LogFormatter& operator=(LogFormatter&&) = delete;

    /**
     * @brief ctor.
     */
    explicit LogFormatter(std::shared_ptr<LogFormatterConfigParser> configParser = nullptr);

    /**
     * @brief dtor.
     */
    ~LogFormatter() = default;

private:
    /**
     * @brief Returns the current time formatted as HH:MM:SS.
     *
     * @return Current time as string.
     */
    static std::string getCurrentTime();

    std::shared_ptr<LogFormatterConfigParser> m_configParser;
    std::string m_moduleName { "NULL" };
};

} // namespace Helper::Logger
