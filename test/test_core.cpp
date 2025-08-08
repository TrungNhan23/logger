
#include "logger.h"
#include <string>

// Redirect output để test
static std::string output;

extern "C" void logger_platform_output(const char *str) {
    output = str;
}

#include <gtest/gtest.h>

TEST(LoggerTest, LogInfoLevel) {
    logger_init();
    logger_set_level(LOG_LEVEL_INFO);
    log_print(LOG_LEVEL_INFO, "Test info %d", 123);
    EXPECT_NE(output.find("INFO"), std::string::npos);
    EXPECT_NE(output.find("123"), std::string::npos);
}

TEST(LoggerTest, LogFilteredLevel) {
    logger_init();
    logger_set_level(LOG_LEVEL_ERROR);
    output.clear();
    log_print(LOG_LEVEL_INFO, "This should be ignored");
    EXPECT_EQ(output, "");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}