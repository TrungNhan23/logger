#pragma once

#include "Logger.hpp"

#define LOG_DEBUG(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::DEBUG,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::WARNING,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    Helper::Logger::Logger::getInstance().printMessage(Helper::Logger::logLevel::ERROR,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)