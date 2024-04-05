#include "App/SignalHandler.h"

void SignalHandler::setupSignalHandling() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
}

void SignalHandler::signalHandler(int) {
    App::shutdown();
}
