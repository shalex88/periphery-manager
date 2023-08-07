#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/PeripheryManager/CommunicationInterface.h"
#include "../source/PeripheryManager/AbstractPeriphery.h"
#include "../source/PeripheryManager/Sensor.h"
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
            sensor(std::make_shared<Sensor>(communication_interface)) {}
    std::shared_ptr<CommunicationMock> communication_interface;
    std::shared_ptr<Sensor> sensor;
};

TEST_F(SensorTests, AbleInit) {
    EXPECT_EQ(sensor->init(), 0);
}

TEST_F(SensorTests, AbleToDeinit) {
    EXPECT_EQ(sensor->deinit(), 0);
}

TEST_F(SensorTests, AbleToGetStatus) {
    EXPECT_CALL(*communication_interface, write(testing::_))
            .WillOnce(testing::Return(3));
    EXPECT_CALL(*communication_interface, read()).Times(1);

    EXPECT_EQ(sensor->getStatus(), 0);
}

TEST_F(SensorTests, AbleToWriteCommand) {
    EXPECT_CALL(*communication_interface, write(testing::_))
        .WillOnce(testing::Return(3));
    EXPECT_CALL(*communication_interface, read()).Times(1);

    EXPECT_EQ(sensor->getTemperature(), 25);
}