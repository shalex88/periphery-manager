#include "Logger/Logger.h"
#include "App/SignalHandler.h"

void SignalHandler::setupSignalHandling() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGSEGV, signalHandler);
}

void SignalHandler::signalHandler(int signal) {
    if (signal == SIGSEGV) {
        LOG_ERROR("Segmentation fault");
    }

    App::shutdown();
}
