#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "App/App.h"
#include <thread>
#include <future>

class AppTests : public testing::Test {
public:
    void SetUp() override {
    }
};

TEST_F(AppTests, CanRunAndBeStoppedGracefully) {
    std::condition_variable condition;
    std::mutex condition_mutex;
    bool finished = false; // Flag to indicate the thread has finished

    App app;
    std::thread app_thread([&] {
        app.run();

        // Signal that the thread has finished
        {
            const std::lock_guard<std::mutex> lock(condition_mutex);
            finished = true;
        }
        condition.notify_one();
    });

    // Simulate stop condition after a delay
    std::this_thread::sleep_for(std::chrono::seconds(2));
    App::shutdown();

    // Wait for the thread to finish with a timeout
    std::unique_lock<std::mutex> lock(condition_mutex);
    const int64_t timeout{15};
    if(!condition.wait_for(lock, std::chrono::seconds(timeout), [&finished]{return finished;})) {
        if(app_thread.joinable()) {
            app_thread.detach();
            FAIL() << "App::run() did not stop within the expected time limit";
        }
    } else {
        if(app_thread.joinable()) {
            app_thread.join();
        }
        SUCCEED();
    }
}