#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Log.hpp"
#include "FileBackend.h"


TEST(FileBackendTest, Write_CallsWriteMethod)
{
    std::string filename = "testLogFile.txt";

    auto backend = std::make_shared<Helper::Logger::FileBackend>(filename);
    Helper::Logger::Logger::getInstance().addBackend(backend);

    std::string testMessage = "Test log message with file backend";

    LOG_INFO(testMessage);

    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string output = buffer.str();

    EXPECT_NE(output.find(testMessage), std::string::npos);
}