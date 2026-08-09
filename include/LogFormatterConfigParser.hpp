#pragma once // NOLINT(llvm-header-guard)

#include <array>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "LogLevel.hpp"

namespace Helper::Logger
{

/**
 * @struct BackendConfig
 * @brief Configuration for a single log backend.
 */
struct BackendConfig
{
    /**
     * @brief Backend name (e.g., "CONSOLE", "FILE", "NETWORK  ").
     */
    BackendType name { BackendType::CONSOLE };

    /**
     * @brief Whether the backend is enabled.
     */
    bool enabled { true };

    /**
     * @brief Log level for this backend (optional, defaults to inherited level).
     */
    std::optional<std::string> level { std::nullopt };

    /** 
     * @brief File path for file backend.
     */
    std::optional<std::string> path { std::nullopt };

    /**
     * @brief Maximum number of backup files for file rotation.
     */
    std::optional<int> max_backups { std::nullopt };
};

/**
 * @struct LogFormat
 * @brief Represents a fully formatted log record including metadata.
 *
 * This structure contains all contextual information required
 * to represent a log message after formatting.
 *
 * It is typically produced by a LogFormatter and consumed
 * by a log backend (e.g., console, file, network).
 */
struct LogFormat
{
    /**
     * @brief Separator string used between fields in the formatted log.
     */
    std::optional<std::string> m_separator;

    /**
     * @brief Timestamp of the log entry.
     *
     * Expected to be pre-formatted as a human-readable string,
     * e.g., "12:00:00.123".
     */
    bool m_timestamp { false };

    /**
     * @brief String representation of the log severity level.
     *
     * Example values: "DEBUG", "INFO", "WARN", "ERROR".
     */
    bool m_level { false };

    /**
     * @brief Source file where the log was generated.
     *
     * Typically provided using the __FILE__ macro.
     */
    bool m_file { false };

    /**
     * @brief Line number in the source file.
     *
     * Typically provided using the __LINE__ macro.
     */
    bool m_line { false };

    /**
     * @brief Final formatted log message content.
     *
     * Contains the user-provided message after
     * printf-style formatting (if applicable).
     */
    std::string m_message;

    /**
     * @brief Name of the module generating the log.
     */
    bool m_moduleName { false };

    /**
     * @brief ID of the thread generating the log.
     */
    bool m_threadId { false };

};

/**
 * @class LogFormatterConfigParser
 * @brief Parser for log formatter configurations from YAML files.
 *
 * This parser reads YAML configuration files and extracts formatting configuration
 * (fields and separator). The parsed configuration is then used by Logger to format
 * log messages.
 *
 * Expected YAML structure:
 *   logging:
 *     format:
 *       fields:
 *         - timestamp
 *         - level
 *         - module
 *         - message
 *       separator: " | "
 *
 * Supported fields:
 *   - timestamp  : HH:MM:SS format
 *   - level      : DEBUG, INFO, WARNING, ERROR
 *   - module     : Module/category name
 *   - thread_id  : Thread ID
 *   - file       : Source file name
 *   - line       : Source line number
 *   - message    : Log message content
 */
class LogFormatterConfigParser
{
public:
    /**
     * @brief Constructor that parses a YAML configuration file.
     *
     * @param configFilePath Path to the YAML configuration file.
     * @throws std::runtime_error if file cannot be opened or parsed.
     */
    explicit LogFormatterConfigParser(const std::string& configFilePath);

    /**
     * @brief Get the list of fields parsed from configuration.
     *
     * @return Shared pointer to LogFormat containing the fields.
     */
    [[nodiscard]] LogFormat getFields() const;

    /**
     * @brief Get all backend configurations parsed from config file.
     *
     * @return Vector of BackendConfig objects.
     */
    [[nodiscard]] const std::vector<BackendConfig>& getBackendConfigs() const;

    /**
     * @brief Deleted copy constructor.
     */
    LogFormatterConfigParser(const LogFormatterConfigParser&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    LogFormatterConfigParser& operator=(const LogFormatterConfigParser&) = delete;

    /**
     * @brief Deleted move constructor.
     */
    LogFormatterConfigParser(LogFormatterConfigParser&&) noexcept = default;

    /**
     * @brief Deleted move assignment operator.
     */
    LogFormatterConfigParser& operator=(LogFormatterConfigParser&&) noexcept = default;

    /**
     * @brief Destructor.
     */
    ~LogFormatterConfigParser() = default;

private:
    /**
     * @brief Remove quotes from a string if present.
     */
    [[nodiscard]] static std::string removeQuotes(const std::string& str);

    /**
     * @brief Get the indentation level of a line.
     */
    [[nodiscard]] static int getIndentLevel(const std::string& line);

    /**
     * @brief Trim leading and trailing whitespace from a string.
     * @param str Input string to trim.
     * @return Trimmed string.
     */
    [[nodiscard]] static std::string trim(const std::string& str);

    /**
     * @brief Parse fields from the configuration file.
     * @param configFile Input file stream of the configuration file.
     * @return LogFormat containing parsed fields.
     */
    [[nodiscard]] static LogFormat getFieldsByParsing(std::ifstream& configFile);

    /**
     * @brief Parse backend configurations from the configuration file.
     * @param configFile Input file stream of the configuration file.
     * @return Vector of BackendConfig containing parsed backend configurations.
     */
    [[nodiscard]] static std::vector<BackendConfig> getBackendConfigsByParsing(std::ifstream& configFile);

    /**
     * @brief List of field names extracted from configuration.
     */
    LogFormat m_fields;

    /**
     * @brief List of backend configurations extracted from configuration.
     */
    std::vector<BackendConfig> m_backendConfigs;
};

} // namespace Helper::Logger
