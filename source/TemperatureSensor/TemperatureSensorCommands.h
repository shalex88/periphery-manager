#ifndef PERIPHERY_MANAGER_TEMPERATURESENSORCOMMANDS_H
#define PERIPHERY_MANAGER_TEMPERATURESENSORCOMMANDS_H

#include "TasksManager/CommandInterface.h"
#include "TemperatureSensor.h"

class GetTempCommand : public CommandInterface {
public:
    explicit GetTempCommand(std::shared_ptr<TemperatureSensor> sensor) : sensor_(std::move(sensor)) {}
    void execute(std::shared_ptr<InputInterface::Requester> requester) override;
    ~GetTempCommand() override = default;

private:
    std::shared_ptr<TemperatureSensor> sensor_;
};

class StopCommand : public CommandInterface {
public:
    void execute(std::shared_ptr<InputInterface::Requester> requester) override;
    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSORCOMMANDS_H
