#pragma once

#include "Logger.hpp"

#define LOG_DEBUG(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::LOG_DEBUG,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::LOG_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::LOG_ERROR,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)