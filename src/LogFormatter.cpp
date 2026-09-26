#include "LogFormatter.hpp"

#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>
#include <utility>

namespace Helper::Logger
{

const size_t SIZE_OF_TIMESTAMP = 9; // HH:MM:SS + null terminator

LogFormatter::LogFormatter(std::shared_ptr<LogFormatterConfigParser> configParser)
    : m_configParser(std::move(configParser))
{
}

void LogFormatter::appendField(std::vector<std::string>& parts, FieldType field,
                               LogLevel level, const SourceLocation& loc,
                               const std::string& formatted, bool& messageAdded) const
{
    switch (field)
    {
        case FieldType::Timestamp:
            parts.push_back(getCurrentTime());
            break;
        case FieldType::Level:
            parts.push_back(std::string("[") + level._to_string() + "]");
            break;
        case FieldType::Module:
            parts.push_back(m_moduleName);
            break;
        case FieldType::ThreadId:
        {
            std::ostringstream tidStream;
            tidStream << std::this_thread::get_id();
            parts.push_back(tidStream.str());
            break;
        }
        case FieldType::File:
            if (loc.file != nullptr)
            {
                parts.emplace_back(loc.basename());
            }
            break;
        case FieldType::Line:
            if (loc.line > 0)
            {
                parts.push_back(std::to_string(loc.line));
            }
            break;
        case FieldType::Function:
            if (loc.function != nullptr)
            {
                parts.emplace_back(loc.function);
            }
            break;
        case FieldType::Message:
            parts.push_back(formatted);
            messageAdded = true;
            break;
    }
}

std::string LogFormatter::formatWithConfig(LogLevel level, const SourceLocation& loc,
                                           const std::string& formatted) const
{
    const auto& fields = m_configParser->getFields();
    const std::string separator = fields.m_separator.value_or(" ");

    std::vector<std::string> parts;
    bool messageAdded = false;

    for (const auto& field : fields.m_fieldOrder)
    {
        appendField(parts, field, level, loc, formatted, messageAdded);
    }

    if (!messageAdded)
    {
        parts.push_back(formatted);
    }

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

std::string LogFormatter::formatFallback(LogLevel level, const std::string& formatted)
{
    std::ostringstream oss;
    if (level._to_integral() == LogLevel::DEBUG || level._to_integral() == LogLevel::ERROR)
    {
        oss << getCurrentTime() << " ";
    }
    oss << "[" << level._to_string() << "] " << formatted;
    return oss.str();
}

std::string LogFormatter::formatMessage(LogLevel level, const SourceLocation& loc,
                                        const std::string& formatted) const
{
    if (m_configParser)
    {
        return formatWithConfig(level, loc, formatted);
    }
    return formatFallback(level, formatted);
}

std::string LogFormatter::getCurrentTime()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t TIME_NOW = std::chrono::system_clock::to_time_t(now);

    std::tm localTime {};
#if defined(_WIN32)
    localtime_s(&localTime, &TIME_NOW);
#else
    localtime_r(&TIME_NOW, &localTime);
#endif

    std::array<char, SIZE_OF_TIMESTAMP> buffer {}; // HH:MM:SS

    const std::size_t written = std::strftime(buffer.data(), buffer.size(), "%H:%M:%S", &localTime);

    return { buffer.data(), written };
}

} // namespace Helper::Logger
