#include "TemperatureSensorCommands.h"

void GetTempCommand::execute(std::shared_ptr<InputInterface::Requester> requester) {
    requester->input_interface_->sendResponse(requester, "Ack");
    if (raise(SIGTERM) != 0) {
        exit(EXIT_FAILURE);
    }
}

void StopCommand::execute(std::shared_ptr<InputInterface::Requester> requester) {
    requester->input_interface_->sendResponse(requester, "Ack");
    if (raise(SIGTERM) != 0) {
        exit(EXIT_FAILURE);
    }
}
