#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ConsoleBackend.hpp"
#include "Logger.hpp"
#include "LogProvider.hpp"

TEST(LoggerTest, GetInstance_ReturnSameInstance)
{
    // install a default logger via provider
    Helper::Logger::setDefaultLogger(std::make_shared<Helper::Logger::Logger>(""));
    auto instance1 = Helper::Logger::defaultLogger();
    auto instance2 = Helper::Logger::defaultLogger();

    EXPECT_EQ(instance1.get(), instance2.get());
}

TEST(LoggerTest, SetLevel_UpdateLevel)
{
    auto logger = Helper::Logger::defaultLogger();
    ASSERT_NE(logger, nullptr);
    logger->setCurrentLevel(Helper::Logger::LogLevel::DEBUG);

    EXPECT_EQ(logger->getCurrentLevel()._to_integral(), Helper::Logger::LogLevel(Helper::Logger::LogLevel::DEBUG)._to_integral());
}

TEST(LoggerTest, AddBackend_AddConsoleBackend)
{
    auto logger = Helper::Logger::defaultLogger();
    ASSERT_NE(logger, nullptr);
    auto backend = std::make_shared<Helper::Logger::ConsoleBackend>();

    EXPECT_NO_THROW(logger->addBackend(backend));
}