#include "TemperatureSensorCommands.h"
#include <vector>

void GetTempCommand::execute(std::shared_ptr<Requester> requester) {
    std::vector<char> response;
    auto temperature = sensor_->getTemperature();

    if (temperature.has_value()) {
        response.push_back(static_cast<char>(temperature.value()));
    } else {
        std::string nack = "Nack";
        response.assign(nack.begin(), nack.end());
    }

    respond(requester, response);
}

void StopCommand::execute(std::shared_ptr<Requester> requester) {
    const std::string response_str = "Ack";
    const std::vector<char> response(response_str.begin(), response_str.end());
    respond(requester, response);

    if (raise(SIGTERM) != 0) {
        exit(EXIT_FAILURE);
    }
}
