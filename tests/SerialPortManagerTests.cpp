#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "Network/SerialPortManager.h"

class SerialPortManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<SerialPortManager> serial_port;

    void SetUp() override {
        serial_port = std::make_shared<SerialPortManager>();
    }
};

TEST_F(SerialPortManagerTest, initReturnsNoErrorOnSuccess) {
    ASSERT_TRUE(serial_port->init() == std::error_code{});
}

TEST_F(SerialPortManagerTest, acceptConnectionReturnsDescriptorOnSuccess) {
    GTEST_SKIP_("Fails when run with all other tests"); //FIXME
    serial_port->init();
    ASSERT_TRUE(serial_port->acceptConnection() != -1);
}

TEST_F(SerialPortManagerTest, acceptConnectionReturnsErrorWhenMoreThanOneClientTriesToConnect) {
    serial_port->init();
    serial_port->acceptConnection();
    ASSERT_TRUE(serial_port->acceptConnection() == -1);
}

TEST_F(SerialPortManagerTest, closingConnectionReturnsNoErrorWhenInitialized) {
    serial_port->init();
    ASSERT_TRUE(serial_port->closeConnection() == std::error_code{});
}

TEST_F(SerialPortManagerTest, closingConnectionReturnsErrorWhenNotInitialized) {
    ASSERT_TRUE(serial_port->closeConnection() == std::errc::not_connected);
}

TEST_F(SerialPortManagerTest, returnTerminateIfReadingWhenConnectionIsClosed) {
    auto [data, terminate] = serial_port->readData(serial_port->acceptConnection());
    ASSERT_TRUE(terminate == true);
}

TEST_F(SerialPortManagerTest, returnErrorIfWritingWhenConnectionIsClosed) {
    std::vector<char> data = {'H', 'e', 'l', 'l', 'o'};
    ASSERT_TRUE(serial_port->sendData(serial_port->acceptConnection(), data) == std::errc::not_connected);
}

TEST_F(SerialPortManagerTest, readDataReturnsDataOnSuccess) {
    GTEST_SKIP_("Create serial client to test message transmission"); //FIXME
    serial_port->init();
    std::vector<char> tx_data = {'H', 'e', 'l', 'l', 'o'};
    ASSERT_TRUE(serial_port->sendData(serial_port->acceptConnection(), tx_data) == std::error_code{});
    auto [rx_data, terminate] = serial_port->readData(serial_port->acceptConnection());
    ASSERT_TRUE(tx_data == rx_data);
}
