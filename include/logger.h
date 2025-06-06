#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>
#include "logger_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level_t;

void logger_init(void);
void logger_set_level(log_level_t level);

void log_print(log_level_t level, const char *fmt, ...);

#define LOGE(...) log_print(LOG_LEVEL_ERROR, "E: " __VA_ARGS__)
#define LOGW(...) log_print(LOG_LEVEL_WARN,  "W: " __VA_ARGS__)
#define LOGI(...) log_print(LOG_LEVEL_INFO,  "I: " __VA_ARGS__)
#define LOGD(...) log_print(LOG_LEVEL_DEBUG, "D: " __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // LOGGER_H
