#include <memory>
#include <string>

#include "Logger.hpp"
#include "ILogBackend.h"

namespace Helper
{
namespace Logger
{

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::setLevel(logLevel& level)
{
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_level = level;
}

void Logger::addBackend(const std::shared_ptr<ILogBackend>& backend)
{
    std::lock_guard<std::mutex> lock(m_logMutex);
    m_logBackends.push_back(backend);
}

} // namespace Logger
} // namespace Helper