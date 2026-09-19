#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <regex>

#include "ConsoleBackend.hpp"
#include "Log.hpp"

TEST(LogFormatterTest, GetCurrentTime_ReturnsHHMMSSFormat)
{
    using namespace Helper::Logger;
    // ensure a default logger exists via provider
    Helper::Logger::setDefaultLogger(std::make_shared<Helper::Logger::Logger>(""));
    auto logger = Helper::Logger::defaultLogger();
    ASSERT_NE(logger, nullptr);
    logger->setCurrentLevel(LogLevel::DEBUG);
    logger->addBackend(std::make_shared<ConsoleBackend>());

    std::string testMessage = "Test log message";

    testing::internal::CaptureStdout();
    LOG_DEBUG(testMessage);
    std::string output = testing::internal::GetCapturedStdout();

    std::regex timePattern(R"(\d{2}:\d{2}:\d{2})");

    EXPECT_TRUE(std::regex_search(output, timePattern));
}
