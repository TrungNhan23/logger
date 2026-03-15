#include "LogFormatter.hpp"

#include <chrono>
#include <ctime>
#include <sstream>

namespace Helper
{
namespace Logger
{

const size_t SIZE_OF_TIMESTAMP = 9; // HH:MM:SS + null terminator

LogFormatter& LogFormatter::getInstance()
{
    static LogFormatter instance;
    return instance;
}

std::string LogFormatter::getCurrentTime() const
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t TIME_NOW =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &TIME_NOW);
#else
    localtime_r(&TIME_NOW, &localTime);
#endif

    std::array<char, SIZE_OF_TIMESTAMP> buffer{}; // HH:MM:SS

    const std::size_t written =
        std::strftime(buffer.data(),
                      buffer.size(),
                      "%H:%M:%S",
                      &localTime);

    if (written == 0U)
    {
        return {};
    }

    return {buffer.data(), written};
}

} // namespace Logger
} // namespace Helper
