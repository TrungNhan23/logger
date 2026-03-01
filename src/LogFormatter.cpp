#include "LogFormatter.hpp"
#include <chrono>
#include <ctime>
#include <sstream>

namespace Helper
{
namespace Logger
{

const size_t SIZE_OF_BUFFER = 9; // HH:MM:SS + null terminator

LogFormatter& LogFormatter::getInstance()
{
    static LogFormatter instance;
    return instance;
}

std::string LogFormatter::format(logLevel& level,
                                 const std::string& file,
                                 int line,
                                 const std::string& message)
{
    std::ostringstream oss;

    oss << file << ":" << line << " ";

    if (level == logLevel::LOG_DEBUG ||
        level == logLevel::LOG_ERROR)
    {
        oss << getCurrentTime() << " ";
    }

    oss << "[" << levelToString(level) << "] "
        << message;

    return oss.str();
}

std::string LogFormatter::getCurrentTime() const
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time_now =
        std::chrono::system_clock::to_time_t(now);

    std::tm local_tm{};
#if defined(_WIN32)
    localtime_s(&local_tm, &time_now);
#else
    localtime_r(&time_now, &local_tm);
#endif

    std::array<char, SIZE_OF_BUFFER> buffer{}; // HH:MM:SS

    const std::size_t written =
        std::strftime(buffer.data(),
                      buffer.size(),
                      "%H:%M:%S",
                      &local_tm);

    if (written == 0U)
    {
        return {};
    }

    return {buffer.data(), written};
}

const char* LogFormatter::levelToString(logLevel& level) const
{
    switch (level)
    {
      case logLevel::LOG_DEBUG: return "DEBUG";
      case logLevel::LOG_INFO:  return "INFO";
      case logLevel::LOG_WARN:  return "WARNING";
      case logLevel::LOG_ERROR: return "ERROR";
      default:                  return "NONE";
    }
}

} // namespace Logger
} // namespace Helper
