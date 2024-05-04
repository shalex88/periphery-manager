#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "AppInputs/MessageServer.h"
#include "Network/TcpNetworkManager.h"
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/Ethernet.h"

class MessageServerTests : public testing::Test {
public:
    std::shared_ptr<Ethernet> tcp_client;
    std::shared_ptr<MessageServer> message_server;
    std::shared_ptr<Scheduler> scheduler;
    std::shared_ptr<TcpNetworkManager> network_manager;
    std::shared_ptr<CommandDispatcher> dispatcher;
    int port = 12345;

    void SetUp() override {
        scheduler = std::make_shared<Scheduler>();
        scheduler->init();
        dispatcher = std::make_shared<CommandDispatcher>(scheduler);
        network_manager = std::make_shared<TcpNetworkManager>(port);
        message_server = std::make_shared<MessageServer>(dispatcher, network_manager);
        tcp_client = std::make_shared<Ethernet>("127.0.0.1", port);
    }

    void TearDown() override {
        //Provide time for the server to shut down gracefully
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
};

TEST_F(MessageServerTests, ServerCanStartAndStop) {
    EXPECT_EQ(message_server->init(), true);
    EXPECT_EQ(message_server->deinit(), true);
}

TEST_F(MessageServerTests, ServerCanBeConnectedTo) {
    message_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(tcp_client->init(), true);
}

TEST_F(MessageServerTests, ServerCanRecieveMultipleClients) {
    message_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(tcp_client->init(), true);

    auto second_tcp_client = std::make_shared<Ethernet>("127.0.0.1", port);
    EXPECT_EQ(second_tcp_client->init(), true);
}

TEST_F(MessageServerTests, ServerCanBeWriteToAndReadFrom) {
    message_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client->init();

    tcp_client->write(request);

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

TEST_F(MessageServerTests, ServerReturnsNackForUnregisteredCommands) {
    message_server->init();

//    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    tcp_client->init();

    std::string request_str = "test";
    std::vector<uint8_t>request {request_str.begin(),request_str.end()};
    tcp_client->write(request);

    std::string expected_str = "Nack";
    std::vector<uint8_t>expected {expected_str.begin(),expected_str.end()};
    EXPECT_EQ(tcp_client->read(), expected);
}

TEST_F(MessageServerTests, ServerReturnsAckForRegisteredCommands) {
    dispatcher->registerCommand("test", std::make_shared<CommandFake>());
    message_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    tcp_client->init();

    std::string request_str = "test";
    std::vector<uint8_t>request {request_str.begin(),request_str.end()};
    tcp_client->write(request);

    std::string expected_str = "Ack";
    std::vector<uint8_t>expected {expected_str.begin(),expected_str.end()};
    EXPECT_EQ(tcp_client->read(), expected);
}
