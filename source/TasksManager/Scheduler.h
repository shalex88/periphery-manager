#ifndef PERIPHERY_MANAGER_SCHEDULER_H
#define PERIPHERY_MANAGER_SCHEDULER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <memory>
#include "TasksManager/Task.h"

class Scheduler {
public:
    explicit Scheduler(size_t thread_count = 1);
    ~Scheduler();
    void enqueueTask(const std::shared_ptr<ITask>& task);
private:
    std::queue<std::shared_ptr<ITask>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
    std::vector<std::thread> worker_threads_;
    void workerFunction();
};

#endif //PERIPHERY_MANAGER_SCHEDULER_H
