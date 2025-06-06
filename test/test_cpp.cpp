#include "Logger.hpp"

int main() {
    Logger log("Main");
    log.error("Something failed with code %d", 500);
    log.info("Everything's good!");
    return 0;
}
