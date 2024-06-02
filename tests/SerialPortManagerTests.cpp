#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "Network/SerialPortManager.h"

//class ISystemCalls {
//public:
//    virtual ~ISystemCalls() = default;
//    virtual int posix_openpt(int flags) = 0;
//    virtual int grantpt(int fd) = 0;
//    virtual int unlockpt(int fd) = 0;
//    virtual int tcgetattr(int fd, struct termios *termios_p) = 0;
//    virtual int tcsetattr(int fd, int optional_actions, const struct termios *termios_p) = 0;
//};
//
//class SystemCalls : public ISystemCalls {
//public:
//    int posix_openpt(int flags) override {
//        return ::posix_openpt(flags);
//    }
//    int grantpt(int fd) override {
//        return ::grantpt(fd);
//    }
//    int unlockpt(int fd) override {
//        return ::unlockpt(fd);
//    }
//    int tcgetattr(int fd, struct termios *termios_p) override {
//        return ::tcgetattr(fd, termios_p);
//    }
//    int tcsetattr(int fd, int optional_actions, const struct termios *termios_p) override {
//        return ::tcsetattr(fd, optional_actions, termios_p);
//    }
//};
//
//class SerialPortManager {
//public:
//    SerialPortManager(std::shared_ptr<ISystemCalls> system_calls)
//        : system_calls_(std::move(system_calls)) {}
//
//    // ...
//
//private:
//    std::shared_ptr<ISystemCalls> system_calls_;
//    // ...
//};
//
//class MockSystemCalls : public ISystemCalls {
//public:
//    MOCK_METHOD(int, posix_openpt, (int flags), (override));
//    MOCK_METHOD(int, grantpt, (int fd), (override));
//    MOCK_METHOD(int, unlockpt, (int fd), (override));
//    MOCK_METHOD(int, tcgetattr, (int fd, struct termios *termios_p), (override));
//    MOCK_METHOD(int, tcsetattr, (int fd, int optional_actions, const struct termios *termios_p), (override));
//};
//
//TEST_F(SerialPortManagerTest, initFailsWhenCannotOpenPort) {
//    auto mock_system_calls = std::make_shared<MockSystemCalls>();
//    EXPECT_CALL(*mock_system_calls, posix_openpt(_)).WillOnce(Return(-1));
//    SerialPortManager spm(mock_system_calls);
//    EXPECT_NE(spm.init(), std::error_code{});
//}

class SerialPortManagerTest : public ::testing::Test {
protected:
    std::shared_ptr<SerialPortManager> serial_port;

    void SetUp() override {
        serial_port = std::make_shared<SerialPortManager>();
    }
};

TEST_F(SerialPortManagerTest, initReturnsNoErrorOnSuccess) {
    EXPECT_EQ(serial_port->init(), std::error_code{});
}

TEST_F(SerialPortManagerTest, initReturnsErrorOnFail) {
    GTEST_SKIP_("Simulate one of the errors"); //FIXME
    EXPECT_NE(serial_port->init(), std::error_code{});
}

TEST_F(SerialPortManagerTest, acceptConnectionReturnsClientDescriptorOnSuccess) {
    GTEST_SKIP_("Fails when run with all other tests"); //FIXME
    serial_port->init();
    EXPECT_NE(serial_port->acceptConnection(), -1);
}

TEST_F(SerialPortManagerTest, acceptConnectionReturnsErrorWhenMoreThanOneClientTriesToConnect) {
    serial_port->init();
    serial_port->acceptConnection();
    EXPECT_EQ(serial_port->acceptConnection(), -1);
}

TEST_F(SerialPortManagerTest, closingConnectionReturnsNoErrorWhenInitialized) {
    serial_port->init();
    EXPECT_EQ(serial_port->closeConnection(), std::error_code{});
}

TEST_F(SerialPortManagerTest, closingConnectionReturnsErrorWhenNotInitialized) {
    EXPECT_EQ(serial_port->closeConnection(), std::errc::not_connected);
}

TEST_F(SerialPortManagerTest, returnTerminateAndEmptyBufferIfReadingWhenClientIsNotConnected) {
    auto client = serial_port->acceptConnection();
    auto [data, terminate] = serial_port->readData(client);
    EXPECT_EQ(terminate, true);
    EXPECT_EQ(data.empty(), true);
}

TEST_F(SerialPortManagerTest, returnTerminateAndEmptyBufferIfReadingHasFailed) {
    serial_port->init();
    auto client = serial_port->acceptConnection();
    auto [data, terminate] = serial_port->readData(client);
    EXPECT_EQ(terminate, true);
    EXPECT_EQ(data.empty(), true);
}

TEST_F(SerialPortManagerTest, returnErrorIfWritingWhenConnectionIsClosed) {
    std::vector<char> data = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(serial_port->sendData(serial_port->acceptConnection(), data), std::errc::not_connected);
}

TEST_F(SerialPortManagerTest, readDataReturnsDataOnSuccess) {
    GTEST_SKIP_("Create serial client to test message transmission"); //FIXME
    serial_port->init();
    std::vector<char> tx_data = {'H', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(serial_port->sendData(serial_port->acceptConnection(), tx_data), std::error_code{});
    auto [rx_data, terminate] = serial_port->readData(serial_port->acceptConnection());
    EXPECT_EQ(tx_data, rx_data);
}
