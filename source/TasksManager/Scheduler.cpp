#include <utility>
#include "TasksManager/Scheduler.h"

// Worker function that will be run by each thread in the pool.
void Scheduler::workerFunction() {
    while (true) { // Infinite loop to keep threads alive and fetching tasks.
        std::shared_ptr<Task> task; // Pointer to the next task to be executed.
        {
            // Locking the queue with a mutex to ensure thread-safe access.
            std::unique_lock<std::mutex> lock(queue_mutex_);
            // Wait on a condition variable until there is a task available or the scheduler is stopped.
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) return; // If stopping and no tasks left, exit the function (and thus the thread).
            task = tasks_.front(); // Get the next task from the queue.
            tasks_.pop(); // Remove the task from the queue.
        } // The lock is released here as the scope ends.

        task->command_->execute(task->request_src_); // Execute the fetched task.
    }
}

// Function to add a task to the queue.
void Scheduler::enqueueTask(std::shared_ptr<InputInterface> responder, const std::shared_ptr<CommandInterface>& command) {
    {
        // Locking the queue with a mutex to ensure thread-safe access.
        std::unique_lock<std::mutex> lock(queue_mutex_);
        auto task = std::make_shared<Task>(std::move(responder), command);
        tasks_.push(task); // Add the task to the queue.
    }
    condition_.notify_one(); // Notify one worker thread that a task is available.
}

// Destructor to clean up when the Scheduler is being destroyed.
Scheduler::~Scheduler() {
    {
        // Locking the queue with a mutex to safely change the stop flag.
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true; // Set the stop flag to true.
    }
    condition_.notify_all(); // Notify all worker threads to wake up, potentially to stop.
    for (auto &thread: worker_threads_) { // For each worker thread,
        thread.join(); // wait for it to finish execution.
    }
}

// Constructor that initializes the Scheduler with a given number of worker threads.
Scheduler::Scheduler(size_t thread_count) {
    for (size_t i = 0; i < thread_count; ++i) {
        // Create worker threads, each running the workerFunction.
        worker_threads_.emplace_back(&Scheduler::workerFunction, this);
    }
}
