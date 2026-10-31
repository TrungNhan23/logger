#pragma once // NOLINT(llvm-header-guard)

/**
 * @file Log.hpp
 * @brief Convenience logging macros for the Helper::Logger framework.
 *
 * Provides a set of severity-level macros (LOG_DEBUG, LOG_INFO, LOG_WARNING,
 * LOG_ERROR) and an assertion macro (LOG_ASSERTION) that automatically
 * capture the call-site source location (__FILE__, __LINE__, __func__) and
 * dispatch through the default logger obtained from LogProvider.
 *
 * All macros are safe to call even when no logger has been registered;
 * they silently become no-ops in that case (except LOG_ASSERTION, which
 * always aborts on a failed condition regardless of logger availability).
 *
 * Message formatting follows the {fmt} library syntax.
 *
 * @see LogProvider.hpp  For registering the default logger instance.
 * @see Logger.hpp       For the Logger class and printMessage() API.
 * @see SourceLocation.hpp For source location capture.
 */

#include <cstdlib>

#include "LogProvider.hpp"

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

/**
 * @def LOG_DEBUG(fmt, ...)
 * @brief Logs a message at DEBUG severity level.
 *
 * Automatically captures the source file, line number, and function name
 * of the call site. The message is only emitted when the logger's current
 * level is set to DEBUG or lower.
 *
 * @param fmt  Format string ({fmt} syntax).
 * @param ...  Optional format arguments.
 *
 * Usage:
 * @code
 *   LOG_DEBUG("Entering loop iteration {}", i);
 * @endcode
 */
#define LOG_DEBUG(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::DEBUG, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

/**
 * @def LOG_INFO(fmt, ...)
 * @brief Logs a message at INFO severity level.
 *
 * Automatically captures the source file, line number, and function name
 * of the call site. Suitable for general informational messages about
 * application state or progress.
 *
 * @param fmt  Format string ({fmt} syntax).
 * @param ...  Optional format arguments.
 *
 * Usage:
 * @code
 *   LOG_INFO("Server started on port {}", port);
 * @endcode
 */
#define LOG_INFO(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::INFO, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

/**
 * @def LOG_WARNING(fmt, ...)
 * @brief Logs a message at WARNING severity level.
 *
 * Automatically captures the source file, line number, and function name
 * of the call site. Use for potentially harmful situations that do not
 * prevent normal operation but should be investigated.
 *
 * @param fmt  Format string ({fmt} syntax).
 * @param ...  Optional format arguments.
 *
 * Usage:
 * @code
 *   LOG_WARNING("Configuration file not found, using defaults");
 * @endcode
 */
#define LOG_WARNING(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::WARNING, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

/**
 * @def LOG_ERROR(fmt, ...)
 * @brief Logs a message at ERROR severity level.
 *
 * Automatically captures the source file, line number, and function name
 * of the call site. Use for error conditions that may allow the
 * application to continue running but indicate a significant problem.
 *
 * @note ERROR-level messages bypass the NONE log level filter and are
 *       always emitted even when the logger level is set to NONE.
 *
 * @param fmt  Format string ({fmt} syntax).
 * @param ...  Optional format arguments.
 *
 * Usage:
 * @code
 *   LOG_ERROR("Failed to open file: {}", filename);
 * @endcode
 */
#define LOG_ERROR(fmt, ...) \
    do { auto lg = Helper::Logger::defaultLogger(); \
         if (lg) lg->printMessage(Helper::Logger::LogLevel::ERROR, \
             Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
             fmt, ##__VA_ARGS__); } while (0)

/**
 * @def LOG_ASSERTION(condition, fmt, ...)
 * @brief Assertion macro that logs a message and aborts on failure.
 *
 * Evaluates @p condition. If it is false:
 * 1. Logs the provided message at ERROR level with source location.
 * 2. Includes the stringified condition text in the log output.
 * 3. Flushes all logger backends to preserve the message.
 * 4. Calls std::abort() to terminate the process.
 *
 * Unlike standard assert(), this macro remains active in release builds
 * (NDEBUG has no effect) to guarantee crash-safety invariants.
 *
 * If no logger is registered, the macro still aborts but without logging.
 *
 * @param condition  Boolean expression to evaluate.
 * @param fmt        Format string ({fmt} syntax) describing the failure.
 * @param ...        Optional format arguments.
 *
 * Usage:
 * @code
 *   LOG_ASSERTION(ptr != nullptr, "Pointer must not be null");
 *   LOG_ASSERTION(idx < size, "Index {} out of range [0, {})", idx, size);
 * @endcode
 */
#define LOG_ASSERTION(condition, fmt, ...) \
    do { if (!(condition)) { \
        auto lg = Helper::Logger::defaultLogger(); \
        if (lg) { \
            lg->printMessage(Helper::Logger::LogLevel::ERROR, \
                Helper::Logger::SourceLocation{__FILE__, __LINE__, __func__}, \
                "ASSERTION FAILED [" #condition "]: " fmt, ##__VA_ARGS__); \
            lg->flushAll(); \
        } \
        std::abort(); \
    } } while (0)

// NOLINTEND(cppcoreguidelines-macro-usage)
