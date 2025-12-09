#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H

#define LOGGER_DEFAULT_LEVEL LOG_LEVEL_DEBUG

#endif

// #include "logger.h"


// extern void mcu_uart_putchar(char c);

// void logger_platform_output(const char *str) {
//     while (*str) {
//         mcu_uart_putchar(*str++);
//     }
//     mcu_uart_putchar('\n'); // Thêm ký tự xuống dòng nếu cần
// }