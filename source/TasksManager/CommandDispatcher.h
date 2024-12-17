#ifndef PERIPHERY_MANAGER_COMMANDDISPATCHER_H
#define PERIPHERY_MANAGER_COMMANDDISPATCHER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "TasksManager/Scheduler.h"
#include "TasksManager/CommandInterface.h"
#include "TasksManager/Requester.h"
#include "proto/internal_api/api.pb.h"

template <>
struct std::hash<api::CommandRequest> {
    std::size_t operator()(const api::CommandRequest& request) const noexcept {
        return std::hash<std::string>()(request.action());
    }
};

namespace api {
    inline bool operator==(const CommandRequest& lhs, const CommandRequest& rhs) {
        return lhs.action() == rhs.action();
    }
}


class CommandDispatcher {
public:
    explicit CommandDispatcher(std::shared_ptr<Scheduler> scheduler);
    ~CommandDispatcher() = default;
    void registerCommand(const api::CommandRequest& command_name, std::shared_ptr<CommandInterface> command);
    void dispatchCommand(const api::CommandRequest& command_name);
    void dispatchCommand(std::shared_ptr<Requester> requester, const api::CommandRequest& command_name);
private:
    std::unordered_map<api::CommandRequest, std::shared_ptr<CommandInterface>> command_map_{
        10, std::hash<api::CommandRequest>(), std::equal_to<api::CommandRequest>()
    };
    std::shared_ptr<Scheduler> scheduler_;
    std::mutex map_mutex_;
};

#endif //PERIPHERY_MANAGER_COMMANDDISPATCHER_H
