#include <utility>
#include "CommandDispatcher.h"

CommandDispatcher::CommandDispatcher(std::shared_ptr<Scheduler> scheduler) : scheduler_(std::move(scheduler)) {}

void CommandDispatcher::registerCommand(const std::string& command_name, std::shared_ptr<CommandInterface> command) {
    std::lock_guard<std::mutex> lock(map_mutex_);
    command_map_[command_name] = std::move(command);
}

void CommandDispatcher::dispatchCommand(std::shared_ptr<InputInterface> request_src, const std::string& command_name) {

    {
        std::lock_guard<std::mutex> lock(map_mutex_);
        auto it = command_map_.find(command_name);
        if (it != command_map_.end()) {
            scheduler_->enqueueTask(std::move(request_src), it->second);
        } else {
            request_src->sendResponse("Nack");
            LOG_ERROR("Unknown command");
        }
    }
}