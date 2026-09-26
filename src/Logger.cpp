#include "Logger.hpp"
#include "ConsoleBackend.hpp"
#include "FileBackend.hpp"

#include <memory>
#include <string>

namespace Helper::Logger
{

Logger::Logger(std::string configFilePath)
    : m_level { LogLevel::INFO }
    , m_logConfigFilePath { std::move(configFilePath) }
{
    if (!m_logConfigFilePath.empty())
    {
        m_loggerConfigParser = std::make_shared<LogFormatterConfigParser>(m_logConfigFilePath);

        const auto& backendConfigs = m_loggerConfigParser->getBackendConfigs();
        for (const auto& backendConfig : backendConfigs)
        {
            if (!backendConfig.enabled)
            {
                continue;
            }

            LogLevel backendLevel = backendConfig.level.value_or(LogLevel::INFO);

            if (backendConfig.name == +BackendType::CONSOLE)
            {
                m_logBackends.push_back({ backendLevel, std::make_shared<ConsoleBackend>() });
            }
            else if (backendConfig.name == +BackendType::FILE)
            {
                if (backendConfig.path.has_value())
                {
                    m_logBackends.push_back({ backendLevel, std::make_shared<FileBackend>(backendConfig.path.value()) });
                }
                else
                {
                    throw std::runtime_error("File backend requires a valid path in the configuration.");
                }
            }
            else
            {
                // Unsupported backend type, ignore or log a warning
            }
        }
    }

    m_formatter = std::make_unique<LogFormatter>(m_loggerConfigParser);
}

void Logger::setCurrentLevel(LogLevel level)
{
    m_level = level;
}

LogLevel Logger::getCurrentLevel() const
{
    return m_level;
}

void Logger::setModuleName(const std::string& moduleName)
{
    if (m_formatter)
    {
        m_formatter->setModuleName(moduleName);
    }
}

std::string Logger::getModuleName() const
{
    if (m_formatter)
    {
        return m_formatter->getModuleName();
    }
    return "";
}

void Logger::addBackend(const std::shared_ptr<ILogBackend>& backend)
{
    if (backend)
    {
        std::lock_guard<std::mutex> lock(m_logMutex);
        m_logBackends.push_back({ LogLevel::VERBOSE, backend });
    }
}

void Logger::flushAll()
{
    std::lock_guard<std::mutex> lock(m_logMutex);
    for (auto& backendEntry : m_logBackends)
    {
        if (backendEntry.backend)
        {
            backendEntry.backend->flush();
        }
    }
}

} // namespace Helper::Logger
