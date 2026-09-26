#include <gtest/gtest.h>

#include "Log.hpp"
#include "LogFormatter.hpp"
#include "LogFormatterConfigParser.hpp"
#include "Logger.hpp"
#include "LogProvider.hpp"
#include "SourceLocation.hpp"

#include <fstream>
#include <regex>
#include <string>

// ============================================================
// SourceLocation Struct Tests
// ============================================================

TEST(SourceLocationTest, DefaultConstruction_AllFieldsNull)
{
    Helper::Logger::SourceLocation loc;
    EXPECT_EQ(loc.file, nullptr);
    EXPECT_EQ(loc.line, 0);
    EXPECT_EQ(loc.function, nullptr);
}

TEST(SourceLocationTest, ParameterizedConstruction_StoresValues)
{
    Helper::Logger::SourceLocation loc("test.cpp", 42, "myFunc");
    EXPECT_STREQ(loc.file, "test.cpp");
    EXPECT_EQ(loc.line, 42);
    EXPECT_STREQ(loc.function, "myFunc");
}

TEST(SourceLocationTest, ExtractBasename_FullPath)
{
    const char* path = "/home/user/project/src/main.cpp";
    EXPECT_STREQ(Helper::Logger::SourceLocation::extractBasename(path), "main.cpp");
}

TEST(SourceLocationTest, ExtractBasename_FileOnly)
{
    const char* path = "main.cpp";
    EXPECT_STREQ(Helper::Logger::SourceLocation::extractBasename(path), "main.cpp");
}

TEST(SourceLocationTest, ExtractBasename_Nullptr)
{
    EXPECT_STREQ(Helper::Logger::SourceLocation::extractBasename(nullptr), "");
}

TEST(SourceLocationTest, Basename_ReturnsFileOnly)
{
    Helper::Logger::SourceLocation loc("/some/path/to/file.cpp", 10, "func");
    EXPECT_STREQ(loc.basename(), "file.cpp");
}

// ============================================================
// Source Location in Macros Test
// ============================================================

TEST(SourceLocationTest, MacroCapturesFileAndLine)
{
    // Create a config that includes file and line fields
    const std::string configPath = "test_source_loc_config.yaml";
    {
        std::ofstream config(configPath);
        config << "logging:\n"
               << "  format:\n"
               << "    fields:\n"
               << "      - timestamp\n"
               << "      - level\n"
               << "      - file\n"
               << "      - line\n"
               << "      - message\n"
               << "    separator: \" | \"\n"
               << "  backends:\n"
               << "    console:\n"
               << "      enabled: true\n"
               << "      level: DEBUG\n";
    }

    auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    Helper::Logger::setDefaultLogger(logger);

    testing::internal::CaptureStdout();
    LOG_INFO("source location test");
    std::string output = testing::internal::GetCapturedStdout();

    // Should contain the test file name and a line number
    EXPECT_TRUE(output.find("SourceLocationTest.cpp") != std::string::npos)
        << "Expected file name in output, got: " << output;

    // Should contain a line number (digits)
    std::regex lineRegex(R"(\d+)");
    EXPECT_TRUE(std::regex_search(output, lineRegex))
        << "Expected line number in output, got: " << output;

    std::remove(configPath.c_str());
}
