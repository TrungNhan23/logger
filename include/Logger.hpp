#pragma once // NOLINT(llvm-header-guard)

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "ILogBackend.hpp"
#include "LogFormatter.hpp"
#include "LogFormatterConfigParser.hpp"
#include "LogLevel.hpp"

namespace Helper::Logger
{

/**
 * @class Logger
 * @brief Thread-safe logger service that outputs formatted logs to multiple backends.
 *
 * This logger provides:
 * - Log level filtering
 * - printf-style formatted logging
 * - Thread-safe output to multiple backends (console, file, etc.)
 * - Configurable formatting via YAML config file
 *
 * Designed as a service component suitable for Linux or desktop environments.
 * Not intended for ISR or hard real-time environments.
 *
 * @note This class should not define as a singleton. Create one instance and manage it via
 *       dependency injection or a service registry.
 */
class Logger
{
public:
    /**
     * @brief Constructs a Logger instance with configuration from YAML file.
     *
     * @param configFilePath Path to the YAML configuration file containing
     *                        format and backends configuration.
     */
    explicit Logger(std::string configFilePath);

    /**
     * @brief Sets the minimum log level.
     *
     * Messages below this level will be ignored.
     *
     * @param level The minimum severity level to log.
     */
    void setCurrentLevel(LogLevel level);

    /**
     * @brief Get the current log level.
     *
     * @return The current log level.
     */
    [[nodiscard]] LogLevel getCurrentLevel() const;

    /**
     * @brief Adds a log backend to output logs to.
     *
     * @param backend Shared pointer to a log backend instance.
     */
    void addBackend(const std::shared_ptr<ILogBackend>& backend);

    template<typename... Args>
    void printMessage(LogLevel level, const std::string& message, Args&&... args)
    {
        if (m_level._to_integral() == LogLevel::NONE)
        {
            if (level._to_integral() != LogLevel::ERROR)
            {
                return;
            }
        }
        else if (level._to_integral() < m_level._to_integral())
        {
            return;
        }

        if (!m_formatter)
        {
            return;
        }

        auto formattedMessage = m_formatter->format(level, message, std::forward<Args>(args)...);

        std::lock_guard<std::mutex> lock(m_logMutex);
        for (const auto& backendEntry : m_logBackends)
        {
            if (backendEntry.backend && level._to_integral() >= backendEntry.level._to_integral())
            {
                backendEntry.backend->write(formattedMessage);
            }
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;
    ~Logger() = default;

private:
    struct BackendEntry
    {
        LogLevel level;
        std::shared_ptr<ILogBackend> backend;
    };

    /**
     * @brief The current log level threshold.
     */
    LogLevel m_level;

    /**
     * @brief Mutex to ensure thread-safe logging.
     */
    std::mutex m_logMutex;

    /**
    * @brief Path to the YAML configuration file.
    *
    * Used for reference and potential reloading of configuration at runtime.
    */
    std::string m_logConfigFilePath;

    /**
     * @brief List of log backends to output logs to.
     *
     * Each backend implements the ILogBackend interface, allowing for flexible
     * log output (e.g., console, file, network).
     */
    std::vector<BackendEntry> m_logBackends;

    /*
     * @brief Log formatter instance used to format log messages according to
     *        the specified formatting policy.
     */
    std::unique_ptr<LogFormatter> m_formatter;

    /*
     * @brief Configuration parser for initializing logger settings from a YAML file.
     */
    std::shared_ptr<LogFormatterConfigParser> m_loggerConfigParser;
};

} // namespace Helper::Logger
