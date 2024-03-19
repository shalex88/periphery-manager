#ifndef PERIPHERY_MANAGER_COMMAND_H
#define PERIPHERY_MANAGER_COMMAND_H

#include <utility>
#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TcpMessageServer/InputInterface.h"

class CommandInterface {
public:
    virtual void execute(std::shared_ptr<InputInterface> responder) = 0;
    virtual ~CommandInterface() = default;
};

class GetTempCommand : public CommandInterface {
public:
    explicit GetTempCommand(std::shared_ptr<TemperatureSensor> sensor) : sensor_(std::move(sensor)) {}

    void execute(std::shared_ptr<InputInterface> responder) override {
        auto temperature = sensor_->getTemperature();
        responder->sendResponse(std::string(reinterpret_cast<char*>(&temperature)));
    }

    ~GetTempCommand() override = default;

private:
    std::shared_ptr<TemperatureSensor> sensor_;
};

class StopCommand : public CommandInterface {
public:
    void execute(std::shared_ptr<InputInterface> responder) override {
        responder->sendResponse("Ack");
        LOG_INFO("Stop");
        exit(EXIT_SUCCESS);
    }

    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_COMMAND_H
