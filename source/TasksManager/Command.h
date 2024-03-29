#ifndef PERIPHERY_MANAGER_COMMAND_H
#define PERIPHERY_MANAGER_COMMAND_H

#include <utility>
#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TcpMessageServer/InputInterface.h"

class CommandInterface {
public:
    virtual void execute(std::shared_ptr<InputInterface::Requester> requester) = 0;
    virtual ~CommandInterface() = default;
};

class GetTempCommand : public CommandInterface {
public:
    explicit GetTempCommand(std::shared_ptr<TemperatureSensor> sensor) : sensor_(std::move(sensor)) {}

    void execute(std::shared_ptr<InputInterface::Requester> requester) override {
        auto temperature = sensor_->getTemperature();
        requester->input_interface_->sendResponse(requester,
                                std::string(reinterpret_cast<char*>(&temperature)));
    }

    ~GetTempCommand() override = default;

private:
    std::shared_ptr<TemperatureSensor> sensor_;
};

class StopCommand : public CommandInterface {
public:
    void execute(std::shared_ptr<InputInterface::Requester> requester) override {
        requester->input_interface_->sendResponse(requester, "Ack");
        LOG_INFO("Stop");
        exit(EXIT_SUCCESS);
    }

    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_COMMAND_H
