#ifndef PERIPHERY_MANAGER_HWMOCK_H
#define PERIPHERY_MANAGER_HWMOCK_H

#include "Logger/Logger.h"
#include "PeripheryManager/HwInterface.h"

class HwMock : public HwInterface {
public:
    HwMock() = default;
    ~HwMock() override = default;
    std::vector<uint8_t> read() override;
    uint8_t write(const std::vector<uint8_t>& tx_data) override;
    bool init() override;
    bool deinit() override;

private:
    std::vector<uint8_t> last_tx_data_;
};

std::vector<uint8_t> HwMock::read() {
    return last_tx_data_;
}

uint8_t HwMock::write(const std::vector<uint8_t> &tx_data) {
    last_tx_data_ = tx_data;

    return tx_data.size();
}

bool HwMock::init() {
    return true;
}

bool HwMock::deinit() {
    return true;
}


#endif //PERIPHERY_MANAGER_HWMOCK_H
