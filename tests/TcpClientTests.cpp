#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "LoopbackTcpServer.h"
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/TcpClient.h"

// TEST PLAN
// + Can connect to server
// + Can disconnect from server
// + Can send data to server
// + Can read data from server

class TcpClientTests : public testing::Test {
public:
   LoopbackTcpServer tcp_server_;
   std::shared_ptr<TcpClient> tcp_client_;
   int port_ = 12345;

    void SetUp() override {
        tcp_server_.startServer(port_);
        // Wait for the server to be ready
        std::this_thread::sleep_for(std::chrono::seconds(2));
        tcp_client_ = std::make_shared<TcpClient>("127.0.0.1", port_);
    }
};

TEST_F(TcpClientTests, ClientCanConnectAndDisconnect) {
    EXPECT_EQ(tcp_client_->init(), true);
    EXPECT_EQ(tcp_client_->deinit(), true);
}

TEST_F(TcpClientTests, ClientCanWriteAndRead) {
    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client_->init();

    EXPECT_EQ(tcp_client_->write(request), 3);
    EXPECT_EQ(tcp_client_->read(), request);

    tcp_client_->deinit();
}