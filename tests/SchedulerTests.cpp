#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TasksManager/Scheduler.h"

class CommandMock : public CommandInterface {
public:
    MOCK_METHOD(void, execute, (std::shared_ptr<InputInterface::Requester> requester), ());
};

class SchedulerTests : public testing::Test {
public:
    SchedulerTests() :
        scheduler(std::make_shared<Scheduler>(thread_count)) {}
    size_t thread_count = 2;
    std::shared_ptr<Scheduler> scheduler;
};

TEST_F(SchedulerTests, ConstructorSetsThreadCount) {
    EXPECT_EQ(scheduler->getThreadCount(), thread_count);
}

TEST_F(SchedulerTests, InitStartsThreads) {
    scheduler->init();
    EXPECT_EQ(scheduler->getRunningThreadCount(), thread_count);
}

TEST_F(SchedulerTests, DeinitStopsThreads) {
    scheduler->init();
    scheduler->deinit();

    EXPECT_EQ(scheduler->getRunningThreadCount(), 0);
}

TEST_F(SchedulerTests, CanEnqueueTasks) {
    scheduler->init();
    scheduler->enqueueTask(std::make_shared<CommandMock>());

    EXPECT_EQ(scheduler->getTaskQueSize(), 1);
}

TEST_F(SchedulerTests, CanDequeueTasks) {
    scheduler->init();
    scheduler->enqueueTask(std::make_shared<CommandMock>());
    scheduler->deinit();

    EXPECT_EQ(scheduler->getTaskQueSize(), 0);
}
