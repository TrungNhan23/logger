#include "Logger.hpp"
#include "ConsoleBackend.hpp"
#include "FileBackend.hpp"

#include <memory>
#include <string>

namespace Helper
{
namespace Logger
{

Logger::Logger(const std::string& configFilePath)
    : m_formatter { std::make_unique<LogFormatter>() }
    , m_level { LogLevel::INFO }
    , m_logConfigFilePath { configFilePath }
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

            if (backendConfig.name == +BackendType::CONSOLE)
            {
                addBackend(std::make_shared<ConsoleBackend>());
            }
            else if (backendConfig.name == +BackendType::FILE)
            {
                if (backendConfig.path.has_value())
                {
                    addBackend(std::make_shared<FileBackend>(backendConfig.path.value()));
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