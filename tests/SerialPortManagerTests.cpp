#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "Network/SerialPortManager.h"

class SerialPortManagerTest : public ::testing::Test {
public:
    SerialPortManager spm;
};

//TEST_F(SerialPortManagerTest, InitReturnsNoErrorOnSuccess) {
//    auto result = spm.init();
//    EXPECT_TRUE(result.value() == 0);
//}
//
//TEST_F(SerialPortManagerTest, InitReturnsErrorOnFailure) {
//    // Simulate failure
//    // ...
//    auto result = spm.init();
//    EXPECT_TRUE(result.value() != 0);
//}
//
//TEST_F(SerialPortManagerTest, CloseConnectionClosesAllConnections) {
//    spm.init();
//    spm.closeConnection();
//    // Check that all connections are closed
//    // ...
//}
//
//TEST_F(SerialPortManagerTest, ReadDataReturnsDataWhenAvailable) {
//    spm.init();
//    // Simulate data availability
//    // ...
//    auto result = spm.readData(0);
//    EXPECT_FALSE(result.first.empty());
//}
//
//TEST_F(SerialPortManagerTest, ReadDataReturnsEmptyWhenNoData) {
//    spm.init();
//    // Simulate no data
//    // ...
//    auto result = spm.readData(0);
//    EXPECT_TRUE(result.first.empty());
//}
//
//TEST_F(SerialPortManagerTest, SendDataReturnsNoErrorOnSuccess) {
//    spm.init();
//    std::vector<char> data = {'H', 'e', 'l', 'l', 'o'};
//    auto result = spm.sendData(0, data);
//    EXPECT_TRUE(result.value() == 0);
//}
//
//TEST_F(SerialPortManagerTest, SendDataReturnsErrorOnFailure) {
//    spm.init();
//    std::vector<char> data = {'H', 'e', 'l', 'l', 'o'};
//    // Simulate failure
//    // ...
//    auto result = spm.sendData(0, data);
//    EXPECT_TRUE(result.value() != 0);
//}
//
//TEST_F(SerialPortManagerTest, AcceptConnectionReturnsValidFdOnSuccess) {
//    spm.init();
//    auto result = spm.acceptConnection();
//    EXPECT_TRUE(result >= 0);
//}
//
//TEST_F(SerialPortManagerTest, AcceptConnectionReturnsErrorOnFailure) {
//    spm.init();
//    // Simulate failure
//    // ...
//    auto result = spm.acceptConnection();
//    EXPECT_TRUE(result < 0);
//}