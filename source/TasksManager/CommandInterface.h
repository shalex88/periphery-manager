#ifndef PERIPHERY_MANAGER_COMMANDINTERFACE_H
#define PERIPHERY_MANAGER_COMMANDINTERFACE_H

#include <utility>
#include <csignal>
#include "Logger/Logger.h"
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

#endif //PERIPHERY_MANAGER_COMMANDINTERFACE_H
