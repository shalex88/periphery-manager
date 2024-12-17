#include "Logger/Logger.h"
#include "App/App.h"

int main() {
    // Default severity level is Info
    SET_LOG_LEVEL(LoggerInterface::LogLevel::Trace);

    LOG_INFO("{} {}.{}.{}", APP_NAME, APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);

    try {
        App::run();
    } catch (const std::exception& e) {
        LOG_ERROR("{}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}