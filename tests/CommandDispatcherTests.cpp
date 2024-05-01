#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TasksManager/CommandDispatcher.h"
#include "TasksManager/Scheduler.h"

class CommandMock : public CommandInterface {
public:
    MOCK_METHOD1(execute, void(std::shared_ptr<InputInterface::Requester>));
};

class CommandDispatcherTests : public testing::Test {
public:
    CommandDispatcherTests() :
        scheduler(std::make_shared<Scheduler>()),
        command_dispatcher(std::make_shared<CommandDispatcher>(scheduler)),
        command(std::make_shared<CommandMock>()) {
        scheduler->init();
    }

    std::shared_ptr<Scheduler> scheduler;
    std::shared_ptr<CommandDispatcher> command_dispatcher;
    std::shared_ptr<CommandMock> command;
    std::string command_name = "command_mock";
};

TEST_F(CommandDispatcherTests, CanBeCreated) {
    EXPECT_NE(command_dispatcher, nullptr);
}

TEST_F(CommandDispatcherTests, PrintsUnknownCommandForUnregisteredCommand) {
// Save the original cout buffer and redirect cout to a custom string stream
    std::streambuf *original_cout_buffer=std::cout.rdbuf();
    std::ostringstream capture_cout;
    std::cout.rdbuf(capture_cout.rdbuf());

    EXPECT_CALL(*command, execute(testing::_)).Times(0);
    command_dispatcher->dispatchCommand(command_name);

// Restore the original cout buffer
    std::cout.rdbuf(original_cout_buffer);

// Compare the captured output with the expected output
    EXPECT_THAT(capture_cout.str(), ::testing::HasSubstr("Unknown command"));
}

TEST_F(CommandDispatcherTests, CanRegisterCommandOnlyOnce) {
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

TEST_F(CommandDispatcherTests, CanDispatchRegisteredCommand) {
    command_dispatcher->registerCommand(command_name, command);

    EXPECT_CALL(*command, execute(testing::_)).Times(1);
    command_dispatcher->dispatchCommand(command_name);
}