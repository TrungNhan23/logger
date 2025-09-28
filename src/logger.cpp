#include <logger.h>
#include <LogFormatter.h>

void Logger::setLevel(logLevel level)
{
    this->LogLevel = level;
}

void Logger::log(logLevel level, const std::string& message)
{
    if (level > LogLevel) return;
    LogFormatter formatter;
    std::string formatted = formatter.format(level, message);
    for (auto& backend : backends) {
        backend->write(formatted);
    }
}

void Logger::addBackend(std::shared_ptr<ILogBackend> backend)
{
    backends.push_back(backend);
}
