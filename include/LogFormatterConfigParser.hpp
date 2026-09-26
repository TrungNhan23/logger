#pragma once // NOLINT(llvm-header-guard)

#include <algorithm>
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
    std::optional<LogLevel> level { std::nullopt };

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
 * @enum FieldType
 * @brief Enumerates the types of fields that can appear in formatted log output.
 *
 * Used in LogFormat::m_fieldOrder to define the exact sequence of
 * fields in the formatted log string.
 */
enum class FieldType
{
    Timestamp,
    Level,
    Module,
    ThreadId,
    File,
    Line,
    Function,
    Message
};

/**
 * @struct LogFormat
 * @brief Represents the formatting configuration for log output.
 *
 * This structure defines which fields appear in the formatted log
 * and in what order. The field ordering is determined by the
 * sequence of entries in the YAML configuration's `fields:` list.
 *
 * It is typically produced by LogFormatterConfigParser and consumed
 * by LogFormatter to assemble the output string.
 */
struct LogFormat
{
    /**
     * @brief Separator string used between fields in the formatted log.
     */
    std::optional<std::string> m_separator;

    /**
     * @brief Ordered sequence of fields to include in the formatted output.
     *
     * The order of entries in this vector determines the exact order
     * of fields in the formatted log string. Parsed from the YAML
     * `fields:` list preserving declaration order.
     */
    std::vector<FieldType> m_fieldOrder;

    /**
     * @brief Check whether a specific field type is present in the field order.
     *
     * @param ft The field type to check for.
     * @return true if the field is in the order list, false otherwise.
     */
    [[nodiscard]] bool hasField(FieldType ft) const
    {
        return std::find(m_fieldOrder.begin(), m_fieldOrder.end(), ft) != m_fieldOrder.end();
    }
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
