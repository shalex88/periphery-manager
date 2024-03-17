#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TcpMessageServer/TcpMessageServer.h"
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/TcpClient.h"

// TEST PLAN
// + Can start server
// + Can stop server
// - Can read data from client
// - Can send data to client
// - Can handle multiple clients

class TcpServerTests : public testing::Test {
public:
   std::shared_ptr<TcpClient> tcp_client_;
    std::shared_ptr<TcpMessageServer> tcp_server_;
    int port_ = 12345;

    void SetUp() override {
        tcp_server_ = std::make_shared<TcpMessageServer>(port_);
        tcp_client_ = std::make_shared<TcpClient>("127.0.0.1", port_);
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

    tcp_server_->deinit();
}

TEST_F(TcpServerTests, ClientCanWriteAndRead) {
    tcp_server_->init();

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client_->init();

    tcp_client_->write(request);

    // Wait for the server to be ready
    std::this_thread::sleep_for(std::chrono::seconds(2));

    tcp_client_->deinit();
    tcp_server_->deinit();
}