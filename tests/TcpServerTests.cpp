#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "AppInputs/MessageServer.h"
#include "Network/TcpNetworkManager.h"
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/TcpClient.h"

class TcpServerTests : public testing::Test {
public:
    std::shared_ptr<TcpClient> tcp_client;
    std::shared_ptr<MessageServer> tcp_server;
    std::shared_ptr<Scheduler> scheduler;
    std::shared_ptr<TcpNetworkManager> network_manager;
    std::shared_ptr<CommandDispatcher> dispatcher;
    int port = 12345;

    void SetUp() override {
        scheduler = std::make_shared<Scheduler>();
        scheduler->init();
        dispatcher = std::make_shared<CommandDispatcher>(scheduler);
        network_manager = std::make_shared<TcpNetworkManager>(port);
        tcp_server = std::make_shared<MessageServer>(dispatcher, network_manager);
        tcp_client = std::make_shared<TcpClient>("127.0.0.1", port);
    }

    void TearDown() override {
        //Provide time for the server to shut down gracefully
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
};

TEST_F(TcpServerTests, ServerCanStartAndStop) {
    EXPECT_EQ(tcp_server->init(), true);
    EXPECT_EQ(tcp_server->deinit(), true);
}

TEST_F(TcpServerTests, ServerCanBeConnectedTo) {
    tcp_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(tcp_client->init(), true);
}

TEST_F(TcpServerTests, ServerCanRecieveMultipleClients) {
    tcp_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(tcp_client->init(), true);

    auto second_tcp_client = std::make_shared<TcpClient>("127.0.0.1", port);
    EXPECT_EQ(second_tcp_client->init(), true);
}

TEST_F(TcpServerTests, ServerCanBeWriteToAndReadFrom) {
    tcp_server->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client->init();

    tcp_client->write(request);

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));
}


TEST_F(TcpServerTests, ServerReturnsNackForUnregisteredCommands) {
    tcp_server->init();

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

TEST_F(TcpServerTests, ServerReturnsAckForRegisteredCommands) {
    dispatcher->registerCommand("test", std::make_shared<DummyCommand>());
    tcp_server->init();

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
