#include "logger.h"
#include "ConsoleBackend.h"
#include "FileBackend.h"

static logLevel currentLevel = LOG_LEVEL_DEBUG;

int main() 
{
    Logger logger;

    // add console backend
    logger.addBackend(std::make_shared<ConsoleBackend>());

    // add file backend
    logger.addBackend(std::make_shared<FileBackend>("test_log_category.txt"));

    // test log
    logger.log(logLevel::LOG_LEVEL_INFO, "This is INFO");
    logger.log(logLevel::LOG_LEVEL_WARN, "This is WARN");
    logger.log(logLevel::LOG_LEVEL_ERROR, "This is ERROR");
    logger.log(logLevel::LOG_LEVEL_DEBUG, "This is DEBUG");
    return 0;
}