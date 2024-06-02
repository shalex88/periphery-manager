#include "CommandDispatcher.h"
#include <utility>

CommandDispatcher::CommandDispatcher(std::shared_ptr<Scheduler> scheduler) : scheduler_(std::move(scheduler)) {}

void CommandDispatcher::registerCommand(const std::string& command_name, std::shared_ptr<CommandInterface> command) {
    const std::lock_guard<std::mutex> lock(map_mutex_);

    if (command_map_.find(command_name) == command_map_.end()) {
        command_map_[command_name] = std::move(command);
    } else {
        LOG_ERROR("Command '{}' is already registered", command_name);
    }
}

void CommandDispatcher::dispatchCommand(std::shared_ptr<Requester> requester, const std::string& command_name) {
    const std::lock_guard<std::mutex> lock(map_mutex_);

    if (auto it = command_map_.find(command_name); it != command_map_.end()) {
        scheduler_->enqueueTask(std::move(requester), it->second);
    } else {
        LOG_ERROR("Unknown command");
        UnknownCommand unknown_command;
        unknown_command.execute(std::move(requester));
    }
}

void CommandDispatcher::dispatchCommand(const std::string& command_name) {
    const std::lock_guard<std::mutex> lock(map_mutex_);

    if (auto it = command_map_.find(command_name); it != command_map_.end()) {
        scheduler_->enqueueTask(it->second);
    } else {
        LOG_ERROR("Unknown command");
    }
}
