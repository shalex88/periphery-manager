#ifndef PERIPHERY_MANAGER_COMMAND_H
#define PERIPHERY_MANAGER_COMMAND_H

#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensor.h"

class CommandInterface {
public:
    virtual void execute() = 0; // Pure virtual function for task execution
    virtual ~CommandInterface() = default;
};

class GetTempCommand : public CommandInterface {
public:
    GetTempCommand(std::shared_ptr<TemperatureSensor> sensor) : sensor_(sensor) {}

    void execute() override {
        sensor_->getTemperature();
    }

    ~GetTempCommand() override = default;

private:
    std::shared_ptr<TemperatureSensor> sensor_;
};

class StopCommand : public CommandInterface {
public:
    void execute() override {
        LOG_INFO("Stop");
        exit(EXIT_SUCCESS);
    }

    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_COMMAND_H
