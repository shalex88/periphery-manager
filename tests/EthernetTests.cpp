#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "PeripheryManager/Ethernet.h"
#include "LoopbackTcpServer.h"
#include "PeripheryManager/HwInterface.h"

class EthernetTests : public testing::Test {
public:
   LoopbackTcpServer tcp_server;
   std::shared_ptr<Ethernet> tcp_client;
   int port = 12345;

    void SetUp() override {
        tcp_server.startServer(port);
        std::this_thread::sleep_for(std::chrono::seconds(2));

        tcp_client = std::make_shared<Ethernet>("127.0.0.1", port);
    }
};

TEST_F(EthernetTests, ClientCanConnectAndDisconnect) {
    EXPECT_EQ(tcp_client->init(), true);
    EXPECT_EQ(tcp_client->deinit(), true);
}

TEST_F(EthernetTests, ClientCanWriteAndRead) {
    auto request = std::vector<uint8_t>{0, 1, 2};

    tcp_client->init();

    EXPECT_EQ(tcp_client->write(request), 3);
    EXPECT_EQ(tcp_client->read(), request);

    tcp_client->deinit();
}