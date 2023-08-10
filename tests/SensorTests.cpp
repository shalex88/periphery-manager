#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/PeripheryManager/CommunicationInterface.h"
#include "../source/PeripheryManager/AbstractPeriphery.h"
#include "../source/PeripheryManager/TemperatureSensor.h"
#include <memory>

// TEST PLAN
// + Able to init HW
// + Able to deinit HW
// + Able to get HW status
// + Able to send HW command

class CommunicationMock : public CommunicationInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, uint8_t(const std::vector<uint8_t>&));
};

class SensorTests : public testing::Test {
public:
    SensorTests() :
            communication_interface(std::make_shared<CommunicationMock>()),
            sensor(std::make_shared<TemperatureSensor>(communication_interface)) {}
    std::shared_ptr<CommunicationMock> communication_interface;
    std::shared_ptr<TemperatureSensor> sensor;
};

TEST_F(SensorTests, AbleInit) {
    EXPECT_CALL(*communication_interface, write(testing::_)).Times(1)
            .WillOnce(testing::Return(3));
    EXPECT_CALL(*communication_interface, read()).Times(1)
            .WillOnce(testing::Return(std::vector<uint8_t>{0, 1, 2}));

    EXPECT_EQ(sensor->init(), 0);
}

TEST_F(SensorTests, AbleToDeinit) {
    EXPECT_CALL(*communication_interface, write(testing::_)).Times(1)
            .WillOnce(testing::Return(0));
    EXPECT_CALL(*communication_interface, read()).Times(0);

    EXPECT_EQ(sensor->deinit(), 0);
}

TEST_F(SensorTests, AbleToGetStatus) {
    EXPECT_CALL(*communication_interface, write(testing::_)).Times(1)
        .WillOnce(testing::Return(3));
    EXPECT_CALL(*communication_interface, read()).Times(1)
        .WillOnce(testing::Return(std::vector<uint8_t>{0, 1, 2}));

    EXPECT_EQ(sensor->getStatus(), 1);
}

TEST_F(SensorTests, AbleToGetTemperature) {
    EXPECT_CALL(*communication_interface, write(testing::_)).Times(1)
            .WillOnce(testing::Return(3));
    EXPECT_CALL(*communication_interface, read()).Times(1)
            .WillOnce(testing::Return(std::vector<uint8_t>{0, 25, 2}));

    EXPECT_EQ(sensor->getTemperature(), 25);
}