// #include "logger.h"
// #include "logger_config.h"
// #include <stdio.h>
// #include <stdarg.h>

// static log_level_t s_log_level = LOGGER_DEFAULT_LEVEL;

// extern void logger_platform_output(const char *str);

// void logger_init(void) {
//     s_log_level = LOGGER_DEFAULT_LEVEL;
// }

// void logger_set_level(log_level_t level) {
//     s_log_level = level;
// }

// log_level_t logger_get_level(void) {
//     return s_log_level;
// }

// void logger_log(log_level_t level, const char *file, int line, const char *fmt, ...) {
//     if (level > s_log_level) return;

//     const char* level_str = "";
//     switch (level) {
//         case LOG_LEVEL_ERROR: level_str = "ERROR"; break;
//         case LOG_LEVEL_WARN:  level_str = "WARN"; break;
//         case LOG_LEVEL_INFO:  level_str = "INFO"; break;
//         case LOG_LEVEL_DEBUG: level_str = "DEBUG"; break;
//         default: level_str = "UNKNOWN"; break;
//     }

//     char msg[256];
//     va_list args;
//     va_start(args, fmt);
//     vsnprintf(msg, sizeof(msg), fmt, args);
//     va_end(args);

//     char final_msg[320];
//     snprintf(final_msg, sizeof(final_msg), "[%s] %s:%d %s", level_str, file, line, msg);

//     logger_platform_output(final_msg);
// }