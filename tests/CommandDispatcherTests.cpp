#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TasksManager/Scheduler.h"
#include "TasksManager/CommandDispatcher.h"

class CommandMock : public CommandInterface {
public:
    MOCK_METHOD(void, execute, (std::shared_ptr<InputInterface::Requester> requester), ());
};

class SchedulerMock : public Scheduler {
public:
    SchedulerMock() : Scheduler() {};
    MOCK_METHOD(void, workerFunction, (), ()) ;
    MOCK_METHOD(void, init, (), ());
    MOCK_METHOD(void, deinit, (), ());
    MOCK_METHOD(void, enqueueTask, (const std::shared_ptr<CommandInterface>& command), ());
    MOCK_METHOD(void, enqueueTask, (std::shared_ptr<InputInterface::Requester> requester, const std::shared_ptr<CommandInterface>& command), ());
};

class CommandDispatcherTests : public testing::Test {
public:
    CommandDispatcherTests() :
        scheduler(std::make_shared<SchedulerMock>()),
        command_dispatcher(std::make_shared<CommandDispatcher>(scheduler)) {}

    std::shared_ptr<SchedulerMock> scheduler;
    std::shared_ptr<CommandDispatcher> command_dispatcher;
};

TEST_F(CommandDispatcherTests, CanBeCreated) {
    EXPECT_NE(command_dispatcher, nullptr);
}

TEST_F(CommandDispatcherTests, PrintsUnknownCommandForUnregisteredCommand) {
// Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

    command_dispatcher->dispatchCommand("command");

// Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

// Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("Unknown command"));
}

TEST_F(CommandDispatcherTests, CanRegisterCommandOnlyOnce) {
    auto command = std::make_shared<CommandMock>();
    auto command_name = "command_mock";

// Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

    command_dispatcher->registerCommand(command_name, command);
    command_dispatcher->registerCommand(command_name, command);

// Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

// Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("already registered"));
}

// TODO: finish
//TEST_F(CommandDispatcherTests, CanDispatchRegisteredCommand) {
//    auto command = std::make_shared<CommandMock>();
//    auto command_name = "command_mock";
//    command_dispatcher->registerCommand(command_name, command);
//
//    EXPECT_CALL(*scheduler, init()).Times(1);
//    EXPECT_CALL(*scheduler, workerFunction()).Times(1);
//    scheduler->init();
//
////    EXPECT_CALL(*scheduler, enqueueTask(std::shared_ptr<CommandInterface>(command))).Times(1);
//
//    std::cout << "Command registered, dispatching..." << std::endl;
//
//
//    EXPECT_CALL(*scheduler, enqueueTask(testing::_)).Times(1);
//    command_dispatcher->dispatchCommand(command_name);
//
//    std::cout << "Dispatched" << std::endl;
//}


