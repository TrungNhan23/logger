#pragma once
extern "C" {
    #include "logger.h"
}

#include <string>
#include <cstdarg>

class Logger {
public:
    Logger(const std::string& tag = "") : tag_(tag) {}

    void error(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        print(LOG_LEVEL_ERROR, fmt, args);
        va_end(args);
    }

    void warn(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        print(LOG_LEVEL_WARN, fmt, args);
        va_end(args);
    }

    void info(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        print(LOG_LEVEL_INFO, fmt, args);
        va_end(args);
    }

    void debug(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        print(LOG_LEVEL_DEBUG, fmt, args);
        va_end(args);
    }

private:
    void print(log_level_t level, const char* fmt, va_list args) const {
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        if (!tag_.empty()) {
            log_print(level, "[%s] %s", tag_.c_str(), buffer);
        } else {
            log_print(level, "%s", buffer);
        }
    }

    std::string tag_;
};
