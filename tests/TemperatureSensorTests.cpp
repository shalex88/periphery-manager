#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"

class HwMock : public HwInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, uint8_t(const std::vector<uint8_t>&));
    MOCK_METHOD0(init, bool());
    MOCK_METHOD0(deinit, bool());
};

class TemperatureSensorMock : public TemperatureSensor {
public:
    using TemperatureSensor::TemperatureSensor;
    MOCK_METHOD0(enable, bool());
    MOCK_METHOD0(disable, bool());
};

class TemperatureSensorTests : public testing::Test {
public:
    TemperatureSensorTests() :
            hw_interface(std::make_shared<HwMock>()),
            protocol_interface(std::make_shared<TemperatureSensorProtocol>()),
            device(std::make_shared<TemperatureSensorMock>(hw_interface, protocol_interface)) {}

    std::shared_ptr<HwMock> hw_interface;
    std::shared_ptr<TemperatureSensorProtocol> protocol_interface;
    std::shared_ptr<TemperatureSensorMock> device;
};

TEST_F(TemperatureSensorTests, AbleInit) {
    EXPECT_CALL(*device, enable())
            .WillOnce(testing::Return(true));
    EXPECT_CALL(*hw_interface, init())
            .WillOnce(testing::Return(true));

    EXPECT_EQ(device->init(), true);
}

TEST_F(TemperatureSensorTests, DeviceInitFailsIfHwConnectionFails) {
    EXPECT_CALL(*device, enable())
            .WillOnce(testing::Return(true));
    EXPECT_CALL(*hw_interface, init())
            .WillOnce(testing::Return(false));
    EXPECT_CALL(*device, disable())
            .WillOnce(testing::Return(true));


    EXPECT_EQ(device->init(), false);
}

TEST_F(TemperatureSensorTests, DeviceInitFailsIfPowerOnFails) {
    EXPECT_CALL(*device, enable())
            .WillOnce(testing::Return(false));

    EXPECT_EQ(device->init(), false);
}

TEST_F(TemperatureSensorTests, AbleToDeinit) {
    EXPECT_CALL(*device, disable())
            .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*hw_interface, deinit())
            .WillRepeatedly(testing::Return(true));

    EXPECT_EQ(device->deinit(), true);
}

TEST_F(TemperatureSensorTests, DeinitFailsIfHwConnectionFails) {
    EXPECT_CALL(*hw_interface, deinit())
            .WillRepeatedly(testing::Return(false));

    EXPECT_EQ(device->deinit(), false);
}

TEST_F(TemperatureSensorTests, DeinitFailsIfPowerOffFails) {
    EXPECT_CALL(*hw_interface, deinit())
            .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*device, disable())
            .WillRepeatedly(testing::Return(false));

    EXPECT_EQ(device->deinit(), false);
}

TEST_F(TemperatureSensorTests, AbleToGetStatus) {
    std::vector<uint8_t> tx_packet = {'$', 1, 1, 1};
    std::vector<uint8_t> rx_packet = {'$', 1, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
        .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
        .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getStatus(), 1);
}

TEST_F(TemperatureSensorTests, AbleToGetTemperature) {
    std::vector<uint8_t> tx_packet = {'$', 1, 25, 25};
    std::vector<uint8_t> rx_packet = {'$', 1, 25, 25};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getTemperature(), 25);
}

TEST_F(TemperatureSensorTests, AbleToGet2bytesHumidity) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1, 2};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getHumidity(), 257);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenNotAllPacketWasWrittenToHw) {
    EXPECT_CALL(*hw_interface, write(testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenNotAllPacketWasReadFromHw) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenPacketIsTooSmall) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenReceivedPacketChecksumIsWrong) {
    std::vector<uint8_t> tx_packet = {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTests, AbleToGetTemperatureAsync) {
    std::vector<uint8_t> tx_packet = {'$', 1, 25, 25};
    std::vector<uint8_t> rx_packet = {'$', 1, 25, 25};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getTemperatureAsynchronously(), 25);
}

TEST_F(TemperatureSensorTests, ExceptionThrownWhenNotAllPacketWasWrittenToHwWithAsync) {
    EXPECT_CALL(*hw_interface, write(testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_THROW(device->getTemperatureAsynchronously(), std::runtime_error);
}