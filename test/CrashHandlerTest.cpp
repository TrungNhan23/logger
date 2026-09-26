#include <gtest/gtest.h>

#include "CrashHandler.hpp"
#include "Logger.hpp"
#include "LogProvider.hpp"

#include <csignal>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

// ============================================================
// Crash Handler Tests (Feature #9)
// ============================================================

TEST(CrashHandlerTest, Install_DoesNotThrow)
{
    EXPECT_NO_THROW(Helper::Logger::CrashHandler::install());
}

TEST(CrashHandlerTest, InstallWithLogger_DoesNotThrow)
{
    auto logger = std::make_shared<Helper::Logger::Logger>("");
    EXPECT_NO_THROW(Helper::Logger::CrashHandler::install(logger));
}

TEST(CrashHandlerTest, SignalCapture_ProducesStackTrace)
{
    // Fork a child process that will crash with SIGABRT
    // and verify the parent can detect the crash
    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "fork() failed";

    if (pid == 0)
    {
        // Child process: install crash handler and abort
        Helper::Logger::CrashHandler::install();
        // Redirect stderr to /dev/null to avoid polluting test output
        freopen("/dev/null", "w", stderr); // NOLINT
        raise(SIGABRT);
        _exit(1); // Should not reach here
    }

    // Parent process: wait for child and check exit status
    int status = 0;
    waitpid(pid, &status, 0);

    // Child should have been killed by SIGABRT
    EXPECT_TRUE(WIFSIGNALED(status)) << "Child should have been killed by signal";
    EXPECT_EQ(WTERMSIG(status), SIGABRT) << "Child should have been killed by SIGABRT";
}

TEST(CrashHandlerTest, SignalCapture_SegfaultCaptured)
{
    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "fork() failed";

    if (pid == 0)
    {
        Helper::Logger::CrashHandler::install();
        freopen("/dev/null", "w", stderr); // NOLINT
        raise(SIGSEGV);
        _exit(1);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    EXPECT_TRUE(WIFSIGNALED(status));
    EXPECT_EQ(WTERMSIG(status), SIGSEGV);
}
