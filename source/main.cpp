#include "Logger/Logger.h"
#include "App/App.h"

int main() {
    // Default severity level is Info
    SET_LOG_LEVEL(LoggerInterface::LogLevel::Trace);

    LOG_INFO("periphery-manager {}.{}.{}", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);

    App app;
    app.run();

    return EXIT_SUCCESS;
}