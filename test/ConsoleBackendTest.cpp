#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Log.hpp"
#include "ConsoleBackend.h"


TEST(ConsoleBackendTest, Write_CallsWriteMethod)
{
    using ::testing::HasSubstr;
    testing::internal::CaptureStdout();
    
    auto backend = std::make_shared<Helper::Logger::ConsoleBackend>();
    Helper::Logger::Logger::getInstance().addBackend(backend);

    std::string testMessage = "Test log message";

    LOG_INFO(testMessage);
        std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find(testMessage), std::string::npos);
}
