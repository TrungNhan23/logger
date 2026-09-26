#pragma once // NOLINT(llvm-header-guard)

#include <memory>

#include "Logger.hpp"

namespace Helper::Logger
{

/**
 * @class CrashHandler
 * @brief Intercepts fatal signals and captures crash diagnostics.
 *
 * When installed, the crash handler intercepts fatal signals
 * (SIGSEGV, SIGABRT, SIGBUS, SIGFPE, SIGILL), captures a stack trace,
 * flushes all logger backends, and then re-raises the signal for
 * default handling (e.g., core dump generation).
 *
 * Usage:
 *   auto logger = std::make_shared<Logger>("config.yaml");
 *   CrashHandler::install(logger);
 *
 * Or without a logger (stack trace to stderr only):
 *   CrashHandler::install();
 */
class CrashHandler
{
public:
    /**
     * @brief Installs the crash handler with a logger for backend flushing.
     *
     * Registers signal handlers for SIGSEGV, SIGABRT, SIGBUS, SIGFPE, SIGILL.
     * On crash, the handler will:
     * 1. Write crash info and stack trace to stderr
     * 2. Flush all backends via the provided logger
     * 3. Re-raise the signal for default handling
     *
     * @param logger Logger instance to flush on crash.
     */
    static void install(std::shared_ptr<Logger> logger);

    /**
     * @brief Installs the crash handler without a logger.
     *
     * Only outputs crash info and stack trace to stderr.
     */
    static void install();

    CrashHandler() = delete;
    ~CrashHandler() = delete;
    CrashHandler(const CrashHandler&) = delete;
    CrashHandler& operator=(const CrashHandler&) = delete;
    CrashHandler(CrashHandler&&) = delete;
    CrashHandler& operator=(CrashHandler&&) = delete;

private:
    /**
     * @brief Signal handler callback.
     *
     * This function is registered via sigaction and called when a
     * fatal signal is received. It is designed to be as async-signal-safe
     * as possible.
     *
     * @param signum The signal number that was received.
     */
    static void signalHandler(int signum);

    static std::shared_ptr<Logger> s_logger; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};

} // namespace Helper::Logger
