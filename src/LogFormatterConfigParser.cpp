#include "LogFormatterConfigParser.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace Helper::Logger
{

namespace
{
void setFieldByName(LogFormat& format, const std::string& field)
{
    if (field == "timestamp") { format.m_timestamp = true; return; }
    if (field == "level") { format.m_level = true; return; }
    if (field == "file") { format.m_file = true; return; }
    if (field == "line") { format.m_line = true; return; }
    if (field == "module") { format.m_moduleName = true; return; }
    if (field == "thread_id") { format.m_threadId = true; return; }
    // "message" is mandatory and implicitly present
}

bool tryInitBackendFromKey(const std::string& key, BackendConfig& out)
{
    try
    {
        out = BackendConfig{};
        out.name = BackendType::_from_string_nocase(key.c_str());
        if (out.name == +BackendType::FILE) { out.max_backups = 1; }
        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}

void flushBackend(std::vector<BackendConfig>& backends, bool hasCurrent, const BackendConfig& current)
{
    if (hasCurrent)
    {
        backends.push_back(current);
    }
}

bool parseBackendProperty(BackendConfig& current, const std::string& key, const std::string& value)
{
    if (key == "enabled")
    {
        current.enabled = (value == "true");
        return true;
    }

    if (key == "level")
    {
        current.level = value;
        return true;
    }

    if (key == "path")
    {
        current.path = value;
        return true;
    }

    if (key == "max_backups")
    {
        current.max_backups = std::stoi(value);
        return true;
    }

    return false;
}

} // namespace

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
            setFieldByName(format, field);

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
            flushBackend(backends, hasCurrent, current);
            backendIndent = indent;
            hasCurrent = tryInitBackendFromKey(key, current);
            continue;
        }

        if (indent <= backendIndent)
        {
            flushBackend(backends, hasCurrent, current);
            configFile.seekg(linePos);
            return backends;
        }

        if (!hasCurrent)
        {
            continue;
        }

        parseBackendProperty(current, key, value);
    }

    flush();
    return backends;
}

int LogFormatterConfigParser::getIndentLevel(const std::string& line)
{
    size_t firstNonIndent = line.find_first_not_of(" \t");
    size_t rawIndent = (firstNonIndent == std::string::npos) ? line.size() : firstNonIndent;
    return static_cast<int>(rawIndent) / 2; // Assuming 2-space indentation
}

std::string LogFormatterConfigParser::removeQuotes(const std::string& str)
{
    std::string result = trim(str);
    if (result.length() >= 2 && result.front() == '"' && result.back() == '"')
    {
        return result.substr(1, result.length() - 2);
    }
    return result;
}

std::string LogFormatterConfigParser::trim(const std::string& str)
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

} // namespace Helper::Logger
