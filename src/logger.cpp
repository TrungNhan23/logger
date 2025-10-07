#include <logger.h>
#include <LogFormatter.h>
#include "ILogBackend.h"
#include <string>
#include <memory>

void Logger::set_level(logLevel level)
{
    this->LogLevel = level;
}

void Logger::log(logLevel level, const std::string& message)
{
    if (level > LogLevel) 
    {
        return;
    }

    auto formatted = LogFormatter::format(level, message);
    for (auto& backend : backends) 
    {
        backend->write(formatted);
    }
}

void Logger::add_backend(std::shared_ptr<ILogBackend> backend)
{
    backends.push_back(backend);
}
