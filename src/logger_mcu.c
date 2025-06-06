#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H

#define LOGGER_DEFAULT_LEVEL  LOG_LEVEL_DEBUG

#endif


#include "logger_config.h"
#include <stdint.h>

// NOTE: You must implement this for your specific MCU (e.g., HAL_UART_Transmit)

void logger_platform_output(const char *str) {
    while (*str) {
        mcu_uart_putchar(*str++);
    }
}
