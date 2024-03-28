#ifndef PERIPHERY_MANAGER_SCHEDULER_H
#define PERIPHERY_MANAGER_SCHEDULER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <utility>
#include <vector>
#include <memory>
#include "TasksManager/Command.h"
#include "TcpMessageServer/InputInterface.h"

class Scheduler {
public:
    explicit Scheduler(size_t thread_count = 1);
    ~Scheduler();
    void enqueueTask(std::shared_ptr<InputInterface> responder, const std::shared_ptr<CommandInterface>& command);
private:
    struct Task {
        std::shared_ptr<InputInterface> requester_;
        std::shared_ptr<CommandInterface> command_;
        Task(std::shared_ptr<InputInterface> requester, std::shared_ptr<CommandInterface> command)
                : requester_(std::move(requester)), command_(std::move(command)) {}
    };
    std::queue<std::shared_ptr<Task>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
    std::vector<std::thread> worker_threads_;
    void workerFunction();
};

#endif //PERIPHERY_MANAGER_SCHEDULER_H
