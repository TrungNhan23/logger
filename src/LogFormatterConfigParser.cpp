#include "LogFormatterConfigParser.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace Helper
{
namespace Logger
{

LogFormatterConfigParser::LogFormatterConfigParser(const std::string& configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Failed to open config file: " + configFilePath);
    }

    m_fields = getFieldsByParsing(configFile);
    m_backendConfigs = getBackendConfigsByParsing(configFile);

    if (m_backendConfigs.empty())
    {
        throw std::runtime_error("No valid configurations found in config file: " + configFilePath);
    }
}

LogFormat LogFormatterConfigParser::getFields() const
{
    return m_fields;
}

const std::vector<BackendConfig>& LogFormatterConfigParser::getBackendConfigs() const
{
    return m_backendConfigs;
}

LogFormat LogFormatterConfigParser::getFieldsByParsing(std::ifstream& configFile)
{
    LogFormat format;
    std::string raw;
    bool inFieldsList = false;

    while (std::getline(configFile, raw))
    {
        size_t indentPos = raw.find_first_not_of(' ');
        if (indentPos == std::string::npos)
        {
            continue;
        }

        std::string content = trim(raw.substr(indentPos));
        if (content.empty() || content[0] == '#')
        {
            continue;
        }

        if (content[0] == '-')
        {
            if (!inFieldsList)
            {
                continue;
            }

            std::string field = removeQuotes(trim(content.substr(1)));

            if (field == "timestamp")      format.m_timestamp = true;
            else if (field == "level")     format.m_level = true;
            else if (field == "file")      format.m_file = true;
            else if (field == "line")      format.m_line = true;
            else if (field == "module")    format.m_moduleName = true;
            else if (field == "thread_id") format.m_threadId = true;
            else if (field == "message")   { /* this field is mandatory, always present, no need to set */ }

            continue;
        }

        size_t colon = content.find(':');
        if (colon == std::string::npos)
        {
            continue;
        }

        std::string key = trim(content.substr(0, colon));
        std::string value = removeQuotes(trim(content.substr(colon + 1)));

        if (key == "fields")
        {
            inFieldsList = true;
            continue;
        }

        if (key == "separator")
        {
            format.m_separator = value;
            inFieldsList = false;
            continue;
        }

        // check if we met the key not related to fields or separator, stop parsing fields
        break;
    }

    return format;
}

std::vector<BackendConfig> LogFormatterConfigParser::getBackendConfigsByParsing(std::ifstream& configFile)
{
    std::vector<BackendConfig> backends;
    BackendConfig current;
    bool hasCurrent = false;
    int backendIndent = -1;

    auto flush = [&]()
    {
        if (hasCurrent)
        {
            backends.push_back(current);
        }
    };

    std::streampos linePos;
    std::string raw;

    while ((linePos = configFile.tellg()), std::getline(configFile, raw))
    {
        size_t indentPos = raw.find_first_not_of(' ');
        if (indentPos == std::string::npos)
        {
            continue;
        }

        std::string content = trim(raw.substr(indentPos));
        if (content.empty() || content[0] == '#')
        {
            continue;
        }

        int indent = static_cast<int>(indentPos);

        size_t colon = content.find(':');
        if (colon == std::string::npos)
        {
            continue;
        }

        std::string key = trim(content.substr(0, colon));
        std::string value = removeQuotes(trim(content.substr(colon + 1)));
        bool isBackendNameLine = value.empty();

        if (isBackendNameLine && (backendIndent == -1 || indent <= backendIndent))
        {
            flush();
            hasCurrent = false;
            backendIndent = indent;

            try
            {
                current = BackendConfig{};
                current.name = BackendType::_from_string_nocase(key.c_str());

                if (current.name == +BackendType::FILE)
                {
                    current.max_backups = 1;
                }

                hasCurrent = true;
            }
            catch (const std::runtime_error&)
            {
                hasCurrent = false;
            }
            continue;
        }

        if (indent <= backendIndent)
        {
            flush();
            configFile.seekg(linePos);
            return backends;
        }

        if (!hasCurrent)
        {
            continue;
        }

        if (key == "enabled")
        {
            current.enabled = (value == "true");
        }
        else if (key == "level")
        {
            current.level = value;
        }
        else if (key == "path")
        {
            current.path = value;
        }
        else if (key == "max_backups")
        {
            current.max_backups = std::stoi(value);
        }
    }

    flush();
    return backends;
}

int LogFormatterConfigParser::getIndentLevel(const std::string& line) const
{
    size_t firstNonIndent = line.find_first_not_of(" \t");
    size_t rawIndent = (firstNonIndent == std::string::npos) ? line.size() : firstNonIndent;
    return static_cast<int>(rawIndent) / 2; // Assuming 2-space indentation
}

std::string LogFormatterConfigParser::removeQuotes(const std::string& str) const
{
    std::string result = trim(str);
    if (result.length() >= 2 && result.front() == '"' && result.back() == '"')
    {
        return result.substr(1, result.length() - 2);
    }
    return result;
}

std::string LogFormatterConfigParser::trim(const std::string& str) const
{
    constexpr const char* whitespace = " \t\n\r\f\v";

    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos)
    {
        return "";
    }

    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

} // namespace Logger
} // namespace Helper
