#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "LoopbackTcpServer.h"
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/TcpClient.h"

class TcpClientTests : public testing::Test {
public:
   LoopbackTcpServer tcp_server;
   std::shared_ptr<TcpClient> tcp_client;
   int port = 12345;

    void SetUp() override {
        tcp_server.startServer(port);
        std::this_thread::sleep_for(std::chrono::seconds(2));

        tcp_client = std::make_shared<TcpClient>("127.0.0.1", port);
    }
};

TEST_F(TcpClientTests, ClientCanConnectAndDisconnect) {
    EXPECT_EQ(tcp_client->init(), true);
    EXPECT_EQ(tcp_client->deinit(), true);
}

TEST_F(TcpClientTests, ClientCanWriteAndRead) {
    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client->init();

    EXPECT_EQ(tcp_client->write(request), 3);
    EXPECT_EQ(tcp_client->read(), request);

    tcp_client->deinit();
}