#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "../source/Camera/DayCamera/DayCamera.h"
#include "../source/Camera/DayCamera/DayCameraProtocol.h"

// TEST PLAN
// + Able to init HW
// + Able to deinit HW
// + Able to zoom in and out
// - Can't zoom outside of the min/max zoom value


class HwMock : public HwInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, uint8_t(const std::vector<uint8_t>&));
};

class DayCameraTests : public testing::Test {
public:
    DayCameraTests() :
            hw_interface(std::make_shared<HwMock>()),
            protocol_interface(std::make_shared<DayCameraProtocol>()),
            sensor(std::make_shared<DayCamera>(hw_interface, protocol_interface)) {}

    std::shared_ptr<HwMock> hw_interface;
    std::shared_ptr<DayCameraProtocol> protocol_interface;
    std::shared_ptr<DayCamera> sensor;
};

TEST_F(DayCameraTests, AbleInit) {
    EXPECT_EQ(sensor->init(), 0);
}

TEST_F(DayCameraTests, AbleToDeinit) {
    EXPECT_EQ(sensor->deinit(), 0);
}

TEST_F(DayCameraTests, AbleToGetStatus) {
    std::vector<uint8_t> tx_packet = {'$', 1, 1, 1};
    std::vector<uint8_t> rx_packet = {'$', 2, 1, 1, 2};

    EXPECT_CALL(*hw_interface, write(tx_packet))
        .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
        .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(sensor->getStatus(), 1);
}

TEST_F(DayCameraTests, AbleToGetZoom) {
    std::vector<uint8_t> tx_packet = {'$', 1, 2, 2};
    std::vector<uint8_t> rx_packet = {'$', 2, 2, 1, 3};

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    EXPECT_EQ(sensor->getZoom(), 1);
}

TEST_F(DayCameraTests, AbleToZoomIn) {
    testing::InSequence seq;

    std::vector<uint8_t> tx_packet = {'$', 1, 2, 2}; // getZoom
    std::vector<uint8_t> rx_packet = {'$', 2, 2, 1, 3}; // getZoom = 1

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    std::vector<uint8_t> tx_packet2 = {'$', 2, 3, 2, 5}; // setZoom(2)
    std::vector<uint8_t> rx_packet2 = {'$', 2, 3, 2, 5}; // setZoom = 2

    EXPECT_CALL(*hw_interface, write(tx_packet2))
            .WillOnce(testing::Return(tx_packet2.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet2));

    EXPECT_EQ(sensor->zoomIn(), 2);
}

TEST_F(DayCameraTests, AbleToZoomOut) {
    testing::InSequence seq;

    std::vector<uint8_t> tx_packet = {'$', 1, 2, 2}; // getZoom
    std::vector<uint8_t> rx_packet = {'$', 2, 2, 2, 4}; // getZoom = 2

    EXPECT_CALL(*hw_interface, write(tx_packet))
            .WillOnce(testing::Return(tx_packet.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet));

    std::vector<uint8_t> tx_packet2 = {'$', 2, 3, 1, 4}; // setZoom(1)
    std::vector<uint8_t> rx_packet2 = {'$', 2, 3, 1, 4}; // setZoom = 1

    EXPECT_CALL(*hw_interface, write(tx_packet2))
            .WillOnce(testing::Return(tx_packet2.size()));
    EXPECT_CALL(*hw_interface, read())
            .WillOnce(testing::Return(rx_packet2));

    EXPECT_EQ(sensor->zoomOut(), 1);
}
