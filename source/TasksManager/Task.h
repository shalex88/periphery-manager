#ifndef PERIPHERY_MANAGER_TASK_H
#define PERIPHERY_MANAGER_TASK_H

#include "Logger/Logger.h"

class ITask {
public:
    virtual void execute() = 0; // Pure virtual function for task execution
    virtual ~ITask() = default;
};

class Task : public ITask{
public:
    void execute() override {
        LOG_INFO("Task1 received");
    }

    ~Task() override = default;
};

class GetTempCommand : public ITask{
public:
    void execute() override {
        LOG_INFO("Get temp");
    }

    ~GetTempCommand() override = default;
};

class StopCommand : public ITask{
public:
    void execute() override {
        LOG_INFO("Stop");
        exit(EXIT_SUCCESS);
    }

    ~StopCommand() override = default;
};

#endif //PERIPHERY_MANAGER_TASK_H
