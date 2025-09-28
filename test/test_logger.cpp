#include <gtest/gtest.h>
#include "logger.h"
#include "ConsoleBackend.h"
#include "FileBackend.h"
#include <fstream>
#include <sstream>
#include <cstdio>

// Helper: read file content
std::string readFile(const std::string& path) 
{
    std::ifstream in(path);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Test case 1: Logger with ConsoleBackend
TEST(LoggerTest, LogToConsole) 
{
    Logger logger;
    logger.setLevel(logLevel::LOG_LEVEL_DEBUG);

    // Tạo backend console
    auto consoleBackend = std::make_shared<ConsoleBackend>();
    logger.addBackend(consoleBackend);

    EXPECT_NO_THROW(logger.log(logLevel::LOG_LEVEL_INFO, "Hello Console"));
}

// Test case 2: Logger with FileBackend
TEST(LoggerTest, LogToFile) 
{
    std::string path = "test_output.log";

    Logger logger;
    logger.setLevel(logLevel::LOG_LEVEL_DEBUG);

    auto fileBackend = std::make_shared<FileBackend>(path);
    logger.addBackend(fileBackend);

    logger.log(logLevel::LOG_LEVEL_INFO, "File Test 123");

    std::string content = readFile(path);
    EXPECT_NE(content.find("File Test 123"), std::string::npos);

    // cleanup
    std::remove(path.c_str());
}

// Test case 3: Log Level Filtering (UC04)
TEST(LoggerTest, LogLevelFilter) 
{
    std::string path = "filter_test.log";

    Logger logger;
    logger.setLevel(logLevel::LOG_LEVEL_WARN); // threshold WARNING

    auto fileBackend = std::make_shared<FileBackend>(path);
    logger.addBackend(fileBackend);

    logger.log(logLevel::LOG_LEVEL_INFO, "This is INFO (should be ignored)");
    logger.log(logLevel::LOG_LEVEL_ERROR, "This is ERROR (should appear)");

    std::string content = readFile(path);
    EXPECT_EQ(content.find("INFO"), std::string::npos);
    EXPECT_NE(content.find("ERROR"), std::string::npos);

    // cleanup
    std::remove(path.c_str());
}
