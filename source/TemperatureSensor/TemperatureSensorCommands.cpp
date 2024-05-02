#include "TemperatureSensorCommands.h"

void GetTempCommand::execute(std::shared_ptr<InputInterface::Requester> requester) {
    auto temperature = sensor_->getTemperature();

    if (temperature.has_value()) {
        requester->source->sendResponse(requester, std::string(reinterpret_cast<char*>(&temperature.value()), 1));
    } else {
        requester->source->sendResponse(requester, "Nack");
    }
}

void StopCommand::execute(std::shared_ptr<InputInterface::Requester> requester) {
    requester->source->sendResponse(requester, "Ack");
    if (raise(SIGTERM) != 0) {
        exit(EXIT_FAILURE);
    }
}
