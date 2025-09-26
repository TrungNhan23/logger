#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include <list>
#include "ILogBackend.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} logLevel;


class Logger : public ILogBackend {
public: 
    void log(logLevel level, std::string message); 
    void setLevel(logLevel level);
    void addBackend(ILogBackend backend);
private:
    logLevel LogLevel; 
    std::list<ILogBackend> backends;
};

#ifdef __cplusplus
}
#endif

#endif // LOGGER_H
