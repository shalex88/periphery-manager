#include "TasksManager/Scheduler.h"

void Scheduler::workerFunction() {
    while (true) {
        std::shared_ptr<ITask> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) return;
            task = tasks_.front();
            tasks_.pop();
        }
        task->execute();
    }
}

void Scheduler::enqueueTask(const std::shared_ptr<ITask>& task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        tasks_.push(task);
    }
    condition_.notify_one();
}

Scheduler::~Scheduler() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (auto &thread: worker_threads_) {
        thread.join();
    }
}

Scheduler::Scheduler(size_t thread_count) {
    for (size_t i = 0; i < thread_count; ++i) {
        worker_threads_.emplace_back(&Scheduler::workerFunction, this);
    }
}