#include <utility>
#include "TasksManager/Scheduler.h"

Scheduler::Scheduler(size_t thread_count) : thread_count_(thread_count) {}

Scheduler::~Scheduler() {
    deinit();
}

void Scheduler::init() {
    for (size_t i = 0; i < thread_count_; ++i) {
        // Create worker threads, each running the workerFunction.
        worker_threads_.emplace_back(&Scheduler::workerFunction, this);
    }
}

void Scheduler::deinit() {
    {
        // Locking the queue with a mutex to safely change the stop flag.
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true; // Set the stop flag to true.
    }
    condition_.notify_all(); // Notify all worker threads to wake up, potentially to stop.
    for (auto& thread: worker_threads_) { // For each worker thread,
        if(thread.joinable()) {
            thread.join(); // wait for it to finish execution.
        }
    }

    worker_threads_.clear();
}

void Scheduler::workerFunction() {
    while (true) { // Infinite loop to keep threads alive and fetching tasks.
        std::shared_ptr<Task> task; // Pointer to the next task to be executed.
        {
            // Locking the queue with a mutex to ensure thread-safe access.
            std::unique_lock<std::mutex> lock(queue_mutex_);
            // Wait on a condition variable until there is a task available or the scheduler is stopped.
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) {
                return;
            }
            task = tasks_.front(); // Get the next task from the queue.
            tasks_.pop(); // Remove the task from the queue.
        } // The lock is released here as the scope ends.

        task->command->execute(task->requester); // Execute the fetched task.
    }
}

void Scheduler::enqueueTask(const std::shared_ptr<CommandInterface>& command) {
    {
        // Locking the queue with a mutex to ensure thread-safe access.
        std::unique_lock<std::mutex> lock(queue_mutex_);
        auto task = std::make_shared<Task>(nullptr, command);
        tasks_.push(task); // Add the task to the queue.
    }
    condition_.notify_one(); // Notify one worker thread that a task is available.
}

void Scheduler::enqueueTask(std::shared_ptr<InputInterface::Requester> requester, const std::shared_ptr<CommandInterface>& command) {
    {
        // Locking the queue with a mutex to ensure thread-safe access.
        std::unique_lock<std::mutex> lock(queue_mutex_);
        auto task = std::make_shared<Task>(std::move(requester), command);
        tasks_.push(task); // Add the task to the queue.
    }
    condition_.notify_one(); // Notify one worker thread that a task is available.
}

size_t Scheduler::getThreadCount() {
    return thread_count_;
}

size_t Scheduler::getRunningThreadCount() {
    return worker_threads_.size();
}

size_t Scheduler::getTaskQueSize() {
    return tasks_.size();
}
