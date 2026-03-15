#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Logger.hpp"
#include "ConsoleBackend.h"


TEST(LoggerTest, GetInstance_ReturnSameInstance) {
    auto &instance1 = Helper::Logger::Logger::getInstance();
    auto &instance2 = Helper::Logger::Logger::getInstance();

    EXPECT_EQ(&instance1, &instance2);
}

TEST(LoggerTest, SetLevel_UpdateLevel)
{
    auto &logger = Helper::Logger::Logger::getInstance();
    logger.setCurrentLevel(Helper::Logger::LogLevel::DEBUG);

    EXPECT_TRUE(logger.getCurrentLevel()._to_integral() ==
                Helper::Logger::LogLevel(Helper::Logger::LogLevel::DEBUG)._to_integral()
    );
}

TEST(LoggerTest, AddBackend_AddConsoleBackend)
{
    auto &logger = Helper::Logger::Logger::getInstance();
    auto backend = std::make_shared<Helper::Logger::ConsoleBackend>();

    logger.addBackend(backend);

    // Since m_logBackends is private, we can't directly check it.
    // Instead, we can add a method to Logger to get the count of backends for testing purposes, or we can rely on the fact that no exceptions are thrown.
    // For this test, we'll just check that no exceptions are thrown when adding a backend.
    EXPECT_NO_THROW(logger.addBackend(backend));    
}