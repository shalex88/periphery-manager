#ifndef PERIPHERY_MANAGER_COMMANDDISPATCHER_H
#define PERIPHERY_MANAGER_COMMANDDISPATCHER_H

#include <unordered_map>
#include <string>
#include <memory>
#include "TasksManager/Command.h"

class CommandDispatcher {
    std::unordered_map<std::string, std::shared_ptr<CommandInterface>> command_map_;

public:
    void registerCommand(const std::string& command_name, std::shared_ptr<CommandInterface> command) {
        command_map_[command_name] = std::move(command);
    }

    void dispatchCommand(const std::string& command_name) {
        auto it = command_map_.find(command_name);
        if (it != command_map_.end()) {
            it->second->execute();
        } else {
            LOG_ERROR("Unknown command");
        }
    }
};


#endif //PERIPHERY_MANAGER_COMMANDDISPATCHER_H
