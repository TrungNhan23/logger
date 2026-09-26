#include "CrashHandler.hpp"

#include <array>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef __linux__
#include <execinfo.h>
#include <unistd.h>
#endif

namespace Helper::Logger
{

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::shared_ptr<Logger> CrashHandler::s_logger = nullptr;

namespace
{

constexpr int MAX_STACK_FRAMES = 64;

/**
 * @brief Async-signal-safe write helper.
 *
 * Writes a string to a file descriptor without using any
 * non-async-signal-safe functions.
 */
void safeWrite(int fd, const char* str)
{
    if (str == nullptr)
    {
        return;
    }
    size_t len = 0;
    while (str[len] != '\0')
    {
        ++len;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    write(fd, str, len);
}

/**
 * @brief Async-signal-safe integer to string conversion.
 *
 * Writes the decimal representation of an integer to a buffer.
 *
 * @param value The integer value to convert.
 * @param buffer Output buffer (must be at least 16 bytes).
 */
void intToStr(int value, char* buffer)
{
    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    constexpr int MAX_DIGITS = 12;
    std::array<char, MAX_DIGITS> tmp{};
    int idx = 0;
    bool negative = value < 0;
    if (negative)
    {
        value = -value;
    }

    while (value > 0 && idx < MAX_DIGITS)
    {
        tmp[idx++] = '0' + static_cast<char>(value % 10); // NOLINT
        value /= 10; // NOLINT
    }

    int pos = 0;
    if (negative)
    {
        buffer[pos++] = '-';
    }
    for (int i = idx - 1; i >= 0; --i)
    {
        buffer[pos++] = tmp[i];
    }
    buffer[pos] = '\0';
}

/**
 * @brief Returns a human-readable name for a signal number.
 */
const char* signalName(int signum)
{
    switch (signum)
    {
        case SIGSEGV: return "SIGSEGV (Segmentation fault)";
        case SIGABRT: return "SIGABRT (Abort)";
        case SIGBUS:  return "SIGBUS (Bus error)";
        case SIGFPE:  return "SIGFPE (Floating point exception)";
        case SIGILL:  return "SIGILL (Illegal instruction)";
        default:      return "Unknown signal";
    }
}

} // namespace

void CrashHandler::install(std::shared_ptr<Logger> logger)
{
    s_logger = std::move(logger);
    install();
}

void CrashHandler::install()
{
    struct sigaction sa{};
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESETHAND; // One-shot: reset to default after first catch

    constexpr std::array<int, 5> signals = { SIGSEGV, SIGABRT, SIGBUS, SIGFPE, SIGILL };
    for (int sig : signals)
    {
        sigaction(sig, &sa, nullptr);
    }
}

void CrashHandler::signalHandler(int signum)
{
    // All output uses write() which is async-signal-safe
    safeWrite(STDERR_FILENO, "\n");
    safeWrite(STDERR_FILENO, "========================================\n");
    safeWrite(STDERR_FILENO, "=== FATAL SIGNAL RECEIVED ===\n");
    safeWrite(STDERR_FILENO, "========================================\n");
    safeWrite(STDERR_FILENO, "Signal: ");
    safeWrite(STDERR_FILENO, signalName(signum));
    safeWrite(STDERR_FILENO, " (");

    std::array<char, 16> sigBuf{};
    intToStr(signum, sigBuf.data());
    safeWrite(STDERR_FILENO, sigBuf.data());
    safeWrite(STDERR_FILENO, ")\n");

#ifdef __linux__
    // Capture stack trace
    safeWrite(STDERR_FILENO, "\n--- Stack Trace ---\n");

    std::array<void*, MAX_STACK_FRAMES> frames{};
    int depth = backtrace(frames.data(), MAX_STACK_FRAMES);

    // backtrace_symbols_fd is the async-signal-safe variant
    backtrace_symbols_fd(frames.data(), depth, STDERR_FILENO);

    safeWrite(STDERR_FILENO, "--- End Stack Trace ---\n\n");
#else
    safeWrite(STDERR_FILENO, "\n(Stack trace not available on this platform)\n\n");
#endif

    // Best-effort flush of logger backends
    // Note: This is not strictly async-signal-safe, but is a best-effort
    // attempt to preserve log data. The signal handler has already been
    // reset (SA_RESETHAND), so a crash during flush will produce a core dump.
    if (s_logger)
    {
        s_logger->flushAll();
    }

    // Re-raise the signal for default handling (core dump, etc.)
    raise(signum);
}

} // namespace Helper::Logger
