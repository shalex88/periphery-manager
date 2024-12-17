#ifndef PERIPHERY_MANAGER_COMMANDINTERFACE_H
#define PERIPHERY_MANAGER_COMMANDINTERFACE_H

#include <utility>
#include <csignal>
#include "Logger/Logger.h"
#include "Requester.h"

class CommandInterface {
public:
    virtual ~CommandInterface() = default;
    virtual void execute(std::shared_ptr<Requester> requester) = 0;
    virtual void respond(std::shared_ptr<Requester> requester, const std::vector<char>& response) {
        std::error_code ec = requester->source->sendData(requester->source_id, response);
        if (ec) {
            LOG_ERROR("Respond wasn't sent");
        }
    }
};

class UnknownCommand : public CommandInterface {
public:
    ~UnknownCommand() override = default;
    void execute(std::shared_ptr<Requester> requester) override {
        std::string response_str = "Nack";
        const std::vector<char> response(response_str.begin(), response_str.end());
        respond(requester, response);
    }
};

class CommandFake : public CommandInterface {
public:
    ~CommandFake() override = default;
    void execute(std::shared_ptr<Requester> requester) override {
        std::string response_str = "Ack";
        const std::vector<char> response(response_str.begin(), response_str.end());
        respond(requester, response);
    }
};

#endif //PERIPHERY_MANAGER_COMMANDINTERFACE_H
