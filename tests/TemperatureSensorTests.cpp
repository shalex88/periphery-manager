#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/TemperatureSensor/TemperatureSensor.h"
#include "../source/TemperatureSensor/TemperatureSensorProtocol.h"

// TEST PLAN
// + Able to init HW
// + Able to deinit HW
// + Able to get from HW 1 byte data
// - Able to get from HW 2 bytes data

class HwMock : public HwInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, uint8_t(const std::vector<uint8_t>&));
};

class TemperatureSensorTests : public testing::Test {
public:
    TemperatureSensorTests() :
            hw_interface(std::make_shared<HwMock>()),
            protocol_interface(std::make_shared<TemperatureSensorProtocol>()),
            sensor(std::make_shared<TemperatureSensor>(hw_interface, protocol_interface)) {}

    std::shared_ptr<HwMock> hw_interface;
    std::shared_ptr<TemperatureSensorProtocol> protocol_interface;
    std::shared_ptr<TemperatureSensor> sensor;
};

TEST_F(TemperatureSensorTests, AbleInit) {
    EXPECT_EQ(sensor->init(), 0);
}

TEST_F(TemperatureSensorTests, AbleToDeinit) {
    EXPECT_EQ(sensor->deinit(), 0);
}

TEST_F(TemperatureSensorTests, AbleToGetStatus) {
    std::vector<uint8_t> tx_packet = {'$', 1, 1, 1};
    std::vector<uint8_t> rx_packet = {'$', 1, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
        .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
        .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(sensor->getStatus(), 1);
}

TEST_F(TemperatureSensorTests, AbleToGetTemperature) {
    std::vector<uint8_t> tx_packet = {'$', 1, 25, 25};
    std::vector<uint8_t> rx_packet = {'$', 1, 25, 25};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(sensor->getTemperature(), 25);
}

TEST_F(TemperatureSensorTests, AbleToGet2bytesHumidity) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1, 2};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(sensor->getHumidity(), 257);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenNowAllPacketWasWrittenToHw) {
    EXPECT_CALL(*hw_interface, write(testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_THROW(sensor->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenNowAllPacketWasReadFromHw) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(sensor->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenPacketIsTooSmall) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(sensor->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenReceivedPacketChecksumIsWrong) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(sensor->getHumidity(), std::runtime_error);
}
