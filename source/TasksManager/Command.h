#ifndef PERIPHERY_MANAGER_COMMAND_H
#define PERIPHERY_MANAGER_COMMAND_H

#include <utility>
#include <csignal>
#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "AppInputs/InputInterface.h"

class CommandInterface {
public:
    virtual void execute(std::shared_ptr<InputInterface::Requester> requester) = 0;
    virtual ~CommandInterface() = default;
};

class DummyCommand : public CommandInterface {
public:
    void execute(std::shared_ptr<InputInterface::Requester> requester) override {
        requester->input_interface_->sendResponse(requester, "Ack");
    }

    ~DummyCommand() override = default;
};

class GetTempCommand : public CommandInterface {
public:
    explicit GetTempCommand(std::shared_ptr<TemperatureSensor> sensor) : sensor_(std::move(sensor)) {}

    void execute(std::shared_ptr<InputInterface::Requester> requester) override {
        auto temperature = sensor_->getTemperature();

        if (temperature.has_value()) {
            requester->input_interface_->sendResponse(requester, std::string(reinterpret_cast<char*>(&temperature.value()), 1));
        } else {
            requester->input_interface_->sendResponse(requester, "Nack");
        }
    }

    ~GetTempCommand() override = default;

private:
    std::shared_ptr<TemperatureSensor> sensor_;
};

class StopCommand : public CommandInterface {
public:
    void execute(std::shared_ptr<InputInterface::Requester> requester) override {
        requester->input_interface_->sendResponse(requester, "Ack");
        if (raise(SIGTERM) != 0) {
            exit(EXIT_FAILURE);
        }
    }

    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_COMMAND_H
