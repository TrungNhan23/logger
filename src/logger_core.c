#include "logger.h"
#include "logger_config.h"
#include <stdio.h>
#include <stdarg.h>

static log_level_t current_level = LOGGER_DEFAULT_LEVEL;

// Platform-specific output
extern void logger_platform_output(const char *str);

void logger_init(void) {
    current_level = LOGGER_DEFAULT_LEVEL;
}

void logger_set_level(log_level_t level) {
    current_level = level;
}

void log_print(log_level_t level, const char *fmt, ...) {
    if (level > current_level) return;

    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    logger_platform_output(buf);
}
