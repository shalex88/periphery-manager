#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(std::shared_ptr<Scheduler> scheduler) : scheduler_(std::move(scheduler)) {}

void CommandDispatcher::registerCommand(const std::string& command_name, std::shared_ptr<CommandInterface> command) {
    std::lock_guard<std::mutex> lock(map_mutex_);
    command_map_[command_name] = std::move(command);
}

void CommandDispatcher::dispatchCommand(const std::string& command_name) {

    {
        std::lock_guard<std::mutex> lock(map_mutex_);
        auto it = command_map_.find(command_name);
        if (it != command_map_.end()) {
            scheduler_->enqueueTask(it->second);
        } else {
            LOG_ERROR("Unknown command");
        }
    }
}