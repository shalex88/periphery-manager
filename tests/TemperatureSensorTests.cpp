#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"

class HwMock : public HwInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, size_t(const std::vector<uint8_t>&));
    MOCK_METHOD0(init, bool());
    MOCK_METHOD0(deinit, bool());
};

class TemperatureSensorMock : public TemperatureSensor {
public:
    using TemperatureSensor::TemperatureSensor;
    MOCK_METHOD0(enable, bool());
    MOCK_METHOD0(disable, bool());
};

class TemperatureSensorTestsBeforeInit : public testing::Test {
public:
    TemperatureSensorTestsBeforeInit() :
            hw_interface(std::make_shared<HwMock>()),
            protocol_interface(std::make_shared<TemperatureSensorProtocol>()),
            device(std::make_shared<TemperatureSensorMock>(hw_interface, protocol_interface)) {}

    std::shared_ptr<HwMock> hw_interface;
    std::shared_ptr<TemperatureSensorProtocol> protocol_interface;
    std::shared_ptr<TemperatureSensorMock> device;

    void TearDown() override {
        EXPECT_CALL(*hw_interface, deinit())
                .WillRepeatedly(testing::Return(true));
    }
};

class TemperatureSensorTestsAfterInit : public TemperatureSensorTestsBeforeInit {
    void SetUp() override {
        EXPECT_CALL(*device, enable())
                .WillRepeatedly(testing::Return(true));
        EXPECT_CALL(*hw_interface, init())
                .WillRepeatedly(testing::Return(true));
        device->init();
    }
    
    void TearDown() override {
        EXPECT_CALL(*device, disable())
                .WillRepeatedly(testing::Return(true));
        EXPECT_CALL(*hw_interface, deinit())
                .WillRepeatedly(testing::Return(true));
    }
};

TEST_F(TemperatureSensorTestsBeforeInit, AbleInit) {
    EXPECT_CALL(*device, enable())
            .WillOnce(testing::Return(true));
    EXPECT_CALL(*hw_interface, init())
            .WillOnce(testing::Return(true));

    EXPECT_EQ(device->init(), true);
}

TEST_F(TemperatureSensorTestsBeforeInit, DeviceInitFailsIfHwConnectionFails) {
    EXPECT_CALL(*device, enable())
            .WillOnce(testing::Return(true));
    EXPECT_CALL(*hw_interface, init())
            .WillOnce(testing::Return(false));
    EXPECT_CALL(*device, disable())
            .WillOnce(testing::Return(true));

    EXPECT_EQ(device->init(), false);
}

TEST_F(TemperatureSensorTestsBeforeInit, DeviceInitFailsIfPowerOnFails) {
    EXPECT_CALL(*device, enable())
            .WillOnce(testing::Return(false));

    EXPECT_EQ(device->init(), false);
}

TEST_F(TemperatureSensorTestsBeforeInit, AbleToDeinitIfNotInited) {
    EXPECT_EQ(device->deinit(), true);
}

TEST_F(TemperatureSensorTestsAfterInit, AbleToDeinitIfInited) {
    EXPECT_CALL(*device, disable())
            .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*hw_interface, deinit())
            .WillRepeatedly(testing::Return(true));

    EXPECT_EQ(device->deinit(), true);
}

TEST_F(TemperatureSensorTestsAfterInit, DeinitFailsIfHwConnectionFails) {
    EXPECT_CALL(*hw_interface, deinit())
            .WillRepeatedly(testing::Return(false));

    EXPECT_EQ(device->deinit(), false);
}

TEST_F(TemperatureSensorTestsAfterInit, DeinitFailsIfPowerOffFails) {
    EXPECT_CALL(*hw_interface, deinit())
            .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*device, disable())
            .WillRepeatedly(testing::Return(false));

    EXPECT_EQ(device->deinit(), false);
}

TEST_F(TemperatureSensorTestsAfterInit, AbleToGetStatus) {
    std::vector<uint8_t> tx_packet {'$', 1, 1, 1};
    std::vector<uint8_t> rx_packet {'$', 1, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
        .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
        .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getStatus(), 1);
}

TEST_F(TemperatureSensorTestsAfterInit, AbleToGetTemperature) {
    std::vector<uint8_t> tx_packet {'$', 1, 25, 25};
    std::vector<uint8_t> rx_packet {'$', 1, 25, 25};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getTemperature(), 25);
}

TEST_F(TemperatureSensorTestsAfterInit, AbleToGet2bytesHumidity) {
    std::vector<uint8_t> tx_packet {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet {'$', 2, 1, 1, 2};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getHumidity(), 257);
}

TEST_F(TemperatureSensorTestsAfterInit, NothingReturnsWhenNotAllPacketWasWrittenToHw) {
    EXPECT_CALL(*hw_interface, write(testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_EQ(device->getHumidity(), std::nullopt);
}

TEST_F(TemperatureSensorTestsAfterInit, ExceptionThrownWhenNotAllPacketWasReadFromHw) {
    std::vector<uint8_t> tx_packet {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet {'$', 2, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTestsAfterInit, ExceptionThrownWhenPacketIsTooSmall) {
    std::vector<uint8_t> tx_packet {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet {1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTestsAfterInit, ExceptionThrownWhenReceivedPacketChecksumIsWrong) {
    std::vector<uint8_t> tx_packet {'$', 2, 1, 1, 2};
    std::vector<uint8_t> rx_packet {'$', 2, 1, 1, 1};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_THROW(device->getHumidity(), std::runtime_error);
}

TEST_F(TemperatureSensorTestsAfterInit, AbleToGetTemperatureAsync) {
    std::vector<uint8_t> tx_packet {'$', 1, 25, 25};
    std::vector<uint8_t> rx_packet {'$', 1, 25, 25};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(device->getTemperatureAsynchronously(), 25);
}

TEST_F(TemperatureSensorTestsAfterInit, ExceptionThrownWhenNotAllPacketWasWrittenToHwWithAsync) {
    EXPECT_CALL(*hw_interface, write(testing::_))
            .WillOnce(testing::Return(1));

    EXPECT_THROW(device->getTemperatureAsynchronously(), std::runtime_error);
}