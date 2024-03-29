#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TcpMessageServer/TcpMessageServer.h"
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/TcpClient.h"

// TEST PLAN
// + Can start server
// + Can stop server
// + Can read data from client
// + Can send data to client
// + Can handle multiple clients

class TcpServerTests : public testing::Test {
public:
    std::shared_ptr<TcpClient> tcp_client_;
    std::shared_ptr<TcpMessageServer> tcp_server_;
    std::shared_ptr<Scheduler> scheduler_;
    std::shared_ptr<CommandDispatcher> dispatcher_;
    int port_ = 12345;

    void SetUp() override {
        scheduler_ = std::make_shared<Scheduler>();
        dispatcher_ = std::make_shared<CommandDispatcher>(scheduler_);
        tcp_server_ = std::make_shared<TcpMessageServer>(port_, dispatcher_);
        tcp_client_ = std::make_shared<TcpClient>("127.0.0.1", port_);
    }

    void TearDown() override {
        //Provide time for the server to shut down gracefully
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
};

TEST_F(TcpServerTests, ServerCanStartAndStop) {
    EXPECT_EQ(tcp_server_->init(), true);
    EXPECT_EQ(tcp_server_->deinit(), true);
}

TEST_F(TcpServerTests, ServerCanBeConnectedTo) {
    tcp_server_->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(tcp_client_->init(), true);
}

TEST_F(TcpServerTests, ServerCanRecieveMultipleClients) {
    tcp_server_->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_EQ(tcp_client_->init(), true);

    auto second_tcp_client = std::make_shared<TcpClient>("127.0.0.1", port_);
    EXPECT_EQ(second_tcp_client->init(), true);
}

TEST_F(TcpServerTests, ServerCanBeWriteToAndReadFrom) {
    tcp_server_->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client_->init();

    tcp_client_->write(request);

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));
}