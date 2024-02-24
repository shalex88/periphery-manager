#ifndef PERIPHERY_MANAGER_HWMOCK_H
#define PERIPHERY_MANAGER_HWMOCK_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "HwInterface.h"

class HwMock : public HwInterface {
public:
    HwMock() = default;
    ~HwMock() override = default;
    std::vector<uint8_t> read() override;
    uint8_t write(const std::vector<uint8_t>& tx_data) override;
private:
    std::vector<uint8_t> last_tx_data_;
};

std::vector<uint8_t> HwMock::read() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return last_tx_data_;
}

uint8_t HwMock::write(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    last_tx_data_ = tx_data;

    return tx_data.size();
}

#endif //PERIPHERY_MANAGER_HWMOCK_H
