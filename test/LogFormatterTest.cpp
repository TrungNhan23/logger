#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <regex>

#include "Log.hpp"
#include "ConsoleBackend.h"

TEST(LogFormatterTest, GetCurrentTime_ReturnsHHMMSSFormat)
{
    using namespace Helper::Logger;
    Logger::getInstance().setCurrentLevel(LogLevel::DEBUG);
    Logger::getInstance().addBackend(std::make_shared<ConsoleBackend>());

    std::string testMessage = "Test log message";

    testing::internal::CaptureStdout();
    LOG_DEBUG(testMessage);
    std::string output = testing::internal::GetCapturedStdout();

    std::regex timePattern(R"(\d{2}:\d{2}:\d{2})");

    EXPECT_TRUE(std::regex_search(output, timePattern));
}
