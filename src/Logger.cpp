#include <memory>
#include <string>

#include "Logger.hpp"

namespace Helper
{
namespace Logger
{

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setCurrentLevel(LogLevel level)
{
    m_level = level;
}

LogLevel Logger::getCurrentLevel() const
{
    return m_level;
}

void Logger::addBackend(const std::shared_ptr<ILogBackend>& backend)
{
    m_logBackends.push_back(backend);
}

} // namespace Logger
} // namespace Helper