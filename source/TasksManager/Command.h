#ifndef PERIPHERY_MANAGER_COMMAND_H
#define PERIPHERY_MANAGER_COMMAND_H

#include "Logger/Logger.h"

class CommandInterface {
public:
    virtual void execute() = 0; // Pure virtual function for task execution
    virtual ~CommandInterface() = default;
};

class Command : public CommandInterface{
public:
    void execute() override {
        LOG_INFO("Task1 received");
    }

    ~Command() override = default;
};

class GetTempCommand : public CommandInterface{
public:
    void execute() override {
        LOG_INFO("Get temp");
    }

    ~GetTempCommand() override = default;
};

class StopCommand : public CommandInterface{
public:
    void execute() override {
        LOG_INFO("Stop");
        exit(EXIT_SUCCESS);
    }

    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_COMMAND_H
