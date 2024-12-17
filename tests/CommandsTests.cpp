#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TasksManager/CommandInterface.h"
#include "Network/SerialPortManager.h"

class InputInterfaceMock : public SerialPortManager {
public:
    MOCK_METHOD2(sendData, std::error_code(int source_id, const std::vector<char>& data));
};

class CommandsTests : public ::testing::Test {
protected:
    std::shared_ptr<InputInterfaceMock> network_interface;
    std::shared_ptr<Requester> requester;

    void SetUp() override {
        network_interface = std::make_shared<InputInterfaceMock>();
        requester = std::make_shared<Requester>(network_interface, 1);
    }
};

TEST_F(CommandsTests, UnknownCommandRespondsWithNack) {
    UnknownCommand command;
    std::string response_str = "Nack";
    std::vector response(response_str.begin(), response_str.end());

    EXPECT_CALL(*network_interface, sendData(testing::_, response)).Times(1);
    command.execute(requester);
}

TEST_F(CommandsTests, CommandFakeRespondsWithAck) {
    CommandFake command;
    std::string response_str = "Ack";
    std::vector response(response_str.begin(), response_str.end());

    EXPECT_CALL(*network_interface, sendData(testing::_, response)).Times(1);
    command.execute(requester);
}