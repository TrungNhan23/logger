#include "logger.h"
#include "ConsoleBackend.h"
#include "FileBackend.h"

int main() {
    Logger logger;

    // add console backend
    logger.addBackend(std::make_shared<ConsoleBackend>());

    // add file backend
    logger.addBackend(std::make_shared<FileBackend>("log.txt"));

    // test log
    logger.log(logLevel::LOG_LEVEL_INFO, "System started");
    logger.log(logLevel::LOG_LEVEL_WARN, "Low battery");
    logger.log(logLevel::LOG_LEVEL_ERROR, "System failure!");

    return 0;
}