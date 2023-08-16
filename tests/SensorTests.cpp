#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/TemperatureSensor/TemperatureSensor.h"

// TEST PLAN
// + Able to init HW
// + Able to deinit HW
// + Able to get from HW 1 byte data
// - Able to get from HW 2 bytes data

class CommunicationMock : public CommunicationInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, uint8_t(const std::vector<uint8_t>&));
};

class ProtocolMock : public ProtocolInterface {
public:
    MOCK_METHOD1(serialize, std::vector<uint8_t>(const std::vector<uint8_t>&));
    MOCK_METHOD1(deserialize, std::vector<uint8_t>(const std::vector<uint8_t>&));
};

class SensorTests : public testing::Test {
public:
    SensorTests() :
            communication_interface(std::make_shared<CommunicationMock>()),
            protocol_interface(std::make_shared<ProtocolMock>()),
            sensor(std::make_shared<TemperatureSensor>(communication_interface, protocol_interface)) {}

    std::shared_ptr<CommunicationMock> communication_interface;
    std::shared_ptr<ProtocolMock> protocol_interface;
    std::shared_ptr<TemperatureSensor> sensor;

    void setupTestExpectations(const std::vector<uint8_t>& rx_data) {
        //Make write() return the number of bytes larger than the data size because serialized data is always larger
        EXPECT_CALL(*protocol_interface, serialize(testing::_))
                .WillOnce(testing::Invoke(
                        [&](const std::vector<uint8_t> &data) {
                            return data;
                        }));
        EXPECT_CALL(*communication_interface, write(testing::_))
                .WillOnce(testing::Invoke(
                        [&](const std::vector<uint8_t> &input) {
                            return static_cast<uint8_t>(input.size()+1);
                        }));

        EXPECT_CALL(*communication_interface, read());
        EXPECT_CALL(*protocol_interface, deserialize(testing::_))
                .WillOnce(testing::Return(rx_data));
    }
};

TEST_F(SensorTests, AbleInit) {
    EXPECT_EQ(sensor->init(), 0);
}

TEST_F(SensorTests, AbleToDeinit) {
    EXPECT_EQ(sensor->deinit(), 0);
}

TEST_F(SensorTests, AbleToGetStatus) {
    std::vector<uint8_t> rx_data = {1};

    setupTestExpectations(rx_data);

    EXPECT_EQ(sensor->getStatus(), 1);
}

TEST_F(SensorTests, AbleToGetTemperature) {
    std::vector<uint8_t> rx_data{25};

    setupTestExpectations(rx_data);

    EXPECT_EQ(sensor->getTemperature(), 25);
}

TEST_F(SensorTests, AbleToGet2bytesHumidity) {
    std::vector<uint8_t> rx_data{1, 1};

    setupTestExpectations(rx_data);

    EXPECT_EQ(sensor->getHumidity(), 257);
}
