#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include <thread>
#include <future>
#include "App/App.h"

class AppTests : public testing::Test {
public:
    void SetUp() override {
    }
};

std::condition_variable cv;
std::mutex cv_m;
bool finished = false; // Flag to indicate the thread has finished

TEST_F(AppTests, CanRunAndBeStoppedGracefully) {
    std::thread app_thread([&] {
        App app;
        app.run();

        // Signal that the thread has finished
        {
            std::lock_guard<std::mutex> lk(cv_m);
            finished = true;
        }
        cv.notify_one();
    });

    // Simulate stop condition after a delay
    std::this_thread::sleep_for(std::chrono::seconds(2));
    raise(SIGTERM);

    // Wait for the thread to finish with a timeout
    std::unique_lock<std::mutex> lk(cv_m);
    if(!cv.wait_for(lk, std::chrono::seconds(15), []{return finished;})) {
        // If the wait_for returns false, the timeout was reached without finishing
        if(app_thread.joinable()) {
            app_thread.detach(); // Detach the thread to avoid destructor hang.
            FAIL() << "App::run() did not stop within the expected time limit";
        }
    } else {
        if(app_thread.joinable()) {
            app_thread.join(); // Make sure to join if the thread finished before detaching
        }
        SUCCEED();
    }
}