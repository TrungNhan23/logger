#include "LogFormatter.h"
#include "logger.h"
#include <string>
#include <chrono>
#include <ctime>

auto LogFormatter::format(logLevel level, const std::string message) -> std::string
{
    auto now = std::chrono::system_clock::now();
    const std::time_t TIME_NOW = std::chrono::system_clock::to_time_t(now);

    std::string lvl;
    switch (level) {
        case logLevel::LOG_LEVEL_NONE: break;
        case logLevel::LOG_LEVEL_DEBUG: lvl = "DEBUG"; break;
        case logLevel::LOG_LEVEL_INFO: lvl = "INFO"; break;
        case logLevel::LOG_LEVEL_WARN: lvl = "WARNING"; break;
        case logLevel::LOG_LEVEL_ERROR: lvl = "ERROR"; break;
    }
    std::string timestamp = std::ctime(&TIME_NOW);
    
    if (!timestamp.empty() && timestamp.back() == '\n') 
    {
        timestamp.pop_back();
    
    }
    return timestamp + " [" + lvl + "] " + message;

}
