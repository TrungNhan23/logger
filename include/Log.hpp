#pragma once // NOLINT(llvm-header-guard)

#include "LogProvider.hpp"

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

#define LOG_DEBUG(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::DEBUG, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

#define LOG_INFO(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::INFO, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

#define LOG_WARNING(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::WARNING, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

#define LOG_ERROR(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::ERROR, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

// NOLINTEND(cppcoreguidelines-macro-usage)
