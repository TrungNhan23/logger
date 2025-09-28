#include "LogFormatter.h"
#include <chrono>
#include <ctime>

auto LogFormatter::format(logLevel level, std::string message) -> std::string
{
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

    std::string lvl;
    switch (level) {
        case logLevel::LOG_LEVEL_NONE: break;
        case logLevel::LOG_LEVEL_DEBUG: lvl = "DEBUG"; break;
        case logLevel::LOG_LEVEL_INFO: lvl = "INFO"; break;
        case logLevel::LOG_LEVEL_WARN: lvl = "WARNING"; break;
        case logLevel::LOG_LEVEL_ERROR: lvl = "ERROR"; break;
    }
    std::string ts = std::ctime(&timeNow);
    
    if (!ts.empty() && ts.back() == '\n') 
    {
        ts.pop_back();
    
    }
    return ts + " [" + lvl + "] " + message;

}
