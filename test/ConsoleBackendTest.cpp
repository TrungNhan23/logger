#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ConsoleBackend.hpp"
#include "Log.hpp"

TEST(ConsoleBackendTest, Write_CallsWriteMethod)
{
    using ::testing::HasSubstr;
    testing::internal::CaptureStdout();

    // ensure a default logger exists via provider
    Helper::Logger::setDefaultLogger(std::make_shared<Helper::Logger::Logger>(""));
    auto logger = Helper::Logger::defaultLogger();
    ASSERT_NE(logger, nullptr);

    auto backend = std::make_shared<Helper::Logger::ConsoleBackend>();
    logger->addBackend(backend);

    std::string testMessage = "Test log message";

    LOG_INFO(testMessage);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(testMessage), std::string::npos);
}
