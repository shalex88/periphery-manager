#include "Logger/Logger.h"
#include "AppManager/AppManager.h"

int main() {
    // Default severity level is Info
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kTrace);

    LOG_INFO("periphery-manager {}.{}.{}", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);

    AppManager app;

    return AppManager::run();
}