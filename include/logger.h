#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>
#include "ILogBackend.h"
#include <memory>
#include <vector>
#include <string>


enum class logLevel
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
};


class Logger 
{
public: 
    Logger() = default;
    void log(logLevel level, const std::string& message); 
    void setLevel(logLevel level);
    void addBackend(std::shared_ptr<ILogBackend> backend);
private:
    logLevel LogLevel = logLevel::LOG_LEVEL_INFO; 
    std::vector<std::shared_ptr<ILogBackend>> backends;
};


#endif // LOGGER_H
