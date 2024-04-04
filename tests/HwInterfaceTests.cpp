#include "gtest/gtest.h"
#include "gmock/gmock.h"
/* Add your project include files here */
#include "PeripheryManager/HwInterface.h"

// TEST PLAN
// + Read 8bit vector from interface
// + Write 8bit vector to the interface

class HwInterfaceMock : public HwInterface {
public:
    MOCK_METHOD0(read, std::vector<uint8_t>());
    MOCK_METHOD1(write, uint8_t(const std::vector<uint8_t>&));
    MOCK_METHOD0(init, bool());
    MOCK_METHOD0(deinit, bool());
};

TEST(HwInterfaceTests, AbleToReadData) {
    auto uart_interface = std::make_shared<HwInterfaceMock>();

    EXPECT_CALL(*uart_interface, read())
            .WillOnce(testing::Return(std::vector<uint8_t>{0, 1, 2}));

    auto rx_data_expected = std::vector<uint8_t>{0, 1, 2};
    EXPECT_EQ(uart_interface->read(), rx_data_expected);
}

TEST(HwInterfaceTests, AbleToWriteData) {
    auto uart_interface = std::make_shared<HwInterfaceMock>();

    auto tx_data = std::vector<uint8_t>{0, 1, 2};

    EXPECT_CALL(*uart_interface, write(testing::_))
            .WillOnce(testing::Return(3));

    EXPECT_EQ(uart_interface->write(tx_data), 3);
}
