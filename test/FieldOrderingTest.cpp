#include <gtest/gtest.h>

#include "Log.hpp"
#include "LogFormatter.hpp"
#include "LogFormatterConfigParser.hpp"
#include "Logger.hpp"
#include "LogProvider.hpp"

#include <fstream>
#include <string>

// ============================================================
// Configurable Field Ordering Tests (Feature #11)
// ============================================================

/**
 * Helper: Creates a temporary YAML config file with specified field order.
 * Returns the config file path.
 */
static std::string createConfigWithFieldOrder(const std::string& filename,
                                               const std::vector<std::string>& fields,
                                               const std::string& separator = " | ")
{
    std::ofstream config(filename);
    config << "logging:\n"
           << "  format:\n"
           << "    fields:\n";
    for (const auto& f : fields)
    {
        config << "      - " << f << "\n";
    }
    config << "    separator: \"" << separator << "\"\n"
           << "  backends:\n"
           << "    console:\n"
           << "      enabled: true\n"
           << "      level: DEBUG\n";
    return filename;
}

TEST(FieldOrderingTest, TimestampModuleLevelMessage_DefaultOrder)
{
    auto configPath = createConfigWithFieldOrder(
        "test_field_order_default.yaml",
        {"timestamp", "module", "level", "message"});

    auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
    logger->setModuleName("TEST");
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    Helper::Logger::setDefaultLogger(logger);

    testing::internal::CaptureStdout();
    LOG_INFO("hello");
    std::string output = testing::internal::GetCapturedStdout();

    // Expected order: timestamp | module | level | message
    // Find positions of each component
    auto posTimestamp = output.find(":");  // part of HH:MM:SS
    auto posModule = output.find("TEST");
    auto posLevel = output.find("[INFO]");
    auto posMessage = output.find("hello");

    EXPECT_NE(posTimestamp, std::string::npos);
    EXPECT_NE(posModule, std::string::npos);
    EXPECT_NE(posLevel, std::string::npos);
    EXPECT_NE(posMessage, std::string::npos);

    // Verify order: timestamp < module < level < message
    EXPECT_LT(posTimestamp, posModule) << "Timestamp should come before module";
    EXPECT_LT(posModule, posLevel) << "Module should come before level";
    EXPECT_LT(posLevel, posMessage) << "Level should come before message";

    std::remove(configPath.c_str());
}

TEST(FieldOrderingTest, LevelTimestampMessage_ReversedOrder)
{
    auto configPath = createConfigWithFieldOrder(
        "test_field_order_reversed.yaml",
        {"level", "timestamp", "message"});

    auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    Helper::Logger::setDefaultLogger(logger);

    testing::internal::CaptureStdout();
    LOG_INFO("reversed");
    std::string output = testing::internal::GetCapturedStdout();

    auto posLevel = output.find("[INFO]");
    auto posTimestamp = output.find(":");  // part of HH:MM:SS
    auto posMessage = output.find("reversed");

    EXPECT_NE(posLevel, std::string::npos);
    EXPECT_NE(posTimestamp, std::string::npos);
    EXPECT_NE(posMessage, std::string::npos);

    // level should come first (before the first colon of the timestamp)
    EXPECT_LT(posLevel, posTimestamp) << "Level should come before timestamp";
    EXPECT_LT(posTimestamp, posMessage) << "Timestamp should come before message";

    std::remove(configPath.c_str());
}

TEST(FieldOrderingTest, MessageOnly_MinimalOutput)
{
    auto configPath = createConfigWithFieldOrder(
        "test_field_order_minimal.yaml",
        {"message"});

    auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    Helper::Logger::setDefaultLogger(logger);

    testing::internal::CaptureStdout();
    LOG_INFO("minimal");
    std::string output = testing::internal::GetCapturedStdout();

    // Should contain the message but not timestamp or level brackets
    EXPECT_NE(output.find("minimal"), std::string::npos);
    // No timestamp (no colon pattern from HH:MM:SS) and no [INFO]
    EXPECT_EQ(output.find("[INFO]"), std::string::npos)
        << "Should not contain level when not in field order";

    std::remove(configPath.c_str());
}

TEST(FieldOrderingTest, AllFields_IncludesEverything)
{
    auto configPath = createConfigWithFieldOrder(
        "test_field_order_all.yaml",
        {"timestamp", "level", "module", "thread_id", "file", "line", "message"});

    auto logger = std::make_shared<Helper::Logger::Logger>(configPath);
    logger->setModuleName("ALLFIELDS");
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);
    Helper::Logger::setDefaultLogger(logger);

    testing::internal::CaptureStdout();
    LOG_DEBUG("all fields test");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("[DEBUG]"), std::string::npos) << "Should contain level";
    EXPECT_NE(output.find("ALLFIELDS"), std::string::npos) << "Should contain module";
    EXPECT_NE(output.find("all fields test"), std::string::npos) << "Should contain message";
    // File name from LOG_DEBUG macro
    EXPECT_NE(output.find("FieldOrderingTest.cpp"), std::string::npos)
        << "Should contain source file name, got: " << output;

    std::remove(configPath.c_str());
}

TEST(FieldOrderingTest, HasField_Query)
{
    Helper::Logger::LogFormat format;
    format.m_fieldOrder = {
        Helper::Logger::FieldType::Timestamp,
        Helper::Logger::FieldType::Level,
        Helper::Logger::FieldType::Message
    };

    EXPECT_TRUE(format.hasField(Helper::Logger::FieldType::Timestamp));
    EXPECT_TRUE(format.hasField(Helper::Logger::FieldType::Level));
    EXPECT_TRUE(format.hasField(Helper::Logger::FieldType::Message));
    EXPECT_FALSE(format.hasField(Helper::Logger::FieldType::Module));
    EXPECT_FALSE(format.hasField(Helper::Logger::FieldType::ThreadId));
    EXPECT_FALSE(format.hasField(Helper::Logger::FieldType::File));
}
