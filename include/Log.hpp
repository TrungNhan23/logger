#pragma once

#include "Logger.hpp"

#define LOG_DEBUG(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::LogLevel::DEBUG,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::LogLevel::INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::LogLevel::WARNING,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::LogLevel::ERROR,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)