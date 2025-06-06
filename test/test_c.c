#include "logger.h"

int main() {
    logger_init();
    LOGE("Error: %d", 404);
    LOGI("Running on C interface");
    return 0;
}
