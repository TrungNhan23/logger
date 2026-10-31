#include <gtest/gtest.h>

#include "Log.hpp"
#include "Logger.hpp"
#include "LogProvider.hpp"

#include <csignal>
#include <fstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

// ============================================================
// LOG_ASSERTION Tests
// ============================================================

TEST(LogAssertionTest, PassingCondition_DoesNotAbort)
{
    // A passing assertion should simply continue execution
    int value = 42;
    LOG_ASSERTION(value == 42, "value should be 42");
    SUCCEED(); // Reached here, assertion passed
}

TEST(LogAssertionTest, FailingCondition_AbortsProcess)
{
    // Fork to test that a failing assertion aborts
    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "fork() failed";

    if (pid == 0)
    {
        // Child process: failing assertion should abort
        freopen("/dev/null", "w", stderr); // NOLINT
        LOG_ASSERTION(false, "this should abort");
        _exit(0); // Should not reach here
    }

    // Parent process: wait for child
    int status = 0;
    waitpid(pid, &status, 0);

    EXPECT_TRUE(WIFSIGNALED(status)) << "Child should have been killed by signal";
    EXPECT_EQ(WTERMSIG(status), SIGABRT) << "Child should have been killed by SIGABRT";
}

TEST(LogAssertionTest, FailingCondition_LogsMessage)
{
    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "fork() failed";

    if (pid == 0)
    {
        // Child: set up logger with file backend, trigger assertion failure
        const std::string configPath = "test_assertion_config.yaml";
        {
            std::ofstream config(configPath);
            config << "logging:\n"
                   << "  format:\n"
                   << "    fields:\n"
                   << "      - level\n"
                   << "      - file\n"
                   << "      - message\n"
                   << "    separator: \" | \"\n"
                   << "  backends:\n"
                   << "    file:\n"
                   << "      enabled: true\n"
                   << "      level: DEBUG\n"
                   << "      path: test_assertion_output.log\n";
        }

        auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
        logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
        Helper::Logger::setDefaultLogger(logger);

        freopen("/dev/null", "w", stderr); // NOLINT

        int badValue = -1;
        LOG_ASSERTION(badValue >= 0, "negative value: {}", badValue);
        _exit(0);
    }

    // Parent: wait for child and verify log file contents
    int status = 0;
    waitpid(pid, &status, 0);

    EXPECT_TRUE(WIFSIGNALED(status));

    // Read the log file produced by the child
    std::ifstream logFile("test_assertion_output.log");
    std::string logContent((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());

    EXPECT_NE(logContent.find("ASSERTION FAILED"), std::string::npos) << "Log should contain 'ASSERTION FAILED', got: " << logContent;
    EXPECT_NE(logContent.find("negative value: -1"), std::string::npos) << "Log should contain the formatted message, got: " << logContent;
    EXPECT_NE(logContent.find("[ERROR]"), std::string::npos) << "Log should be at ERROR level, got: " << logContent;

    // Cleanup
    std::remove("test_assertion_config.yaml");
    std::remove("test_assertion_output.log");
}

TEST(LogAssertionTest, FailingCondition_IncludesConditionText)
{
    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "fork() failed";

    if (pid == 0)
    {
        const std::string configPath = "test_assertion_cond.yaml";
        {
            std::ofstream config(configPath);
            config << "logging:\n"
                   << "  format:\n"
                   << "    fields:\n"
                   << "      - message\n"
                   << "    separator: \" \"\n"
                   << "  backends:\n"
                   << "    file:\n"
                   << "      enabled: true\n"
                   << "      level: DEBUG\n"
                   << "      path: test_assertion_cond_output.log\n";
        }

        auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
        logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
        Helper::Logger::setDefaultLogger(logger);

        freopen("/dev/null", "w", stderr); // NOLINT

        int* ptr = nullptr;
        LOG_ASSERTION(ptr != nullptr, "pointer is null");
        _exit(0);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    std::ifstream logFile("test_assertion_cond_output.log");
    std::string logContent((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());

    // The stringified condition should appear in the log
    EXPECT_NE(logContent.find("ptr != nullptr"), std::string::npos) << "Log should contain the stringified condition, got: " << logContent;
    EXPECT_NE(logContent.find("pointer is null"), std::string::npos) << "Log should contain the user message, got: " << logContent;

    std::remove("test_assertion_cond.yaml");
    std::remove("test_assertion_cond_output.log");
}
