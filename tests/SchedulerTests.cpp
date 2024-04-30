#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TasksManager/Scheduler.h"

class CommandMock : public CommandInterface {
public:
    MOCK_METHOD1(execute, void(std::shared_ptr<InputInterface::Requester>));
};

class SchedulerTests : public testing::Test {
public:
    SchedulerTests() :
        scheduler(std::make_shared<Scheduler>(thread_count)),
        command(std::make_shared<CommandMock>()) {}
    size_t thread_count = 2;
    std::shared_ptr<Scheduler> scheduler;
    std::shared_ptr<CommandMock> command;
};

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
    EXPECT_CALL(*command, execute(testing::_)).Times(1);
    scheduler->enqueueTask(command);
}