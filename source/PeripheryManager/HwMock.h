#ifndef PERIPHERY_MANAGER_HWMOCK_H
#define PERIPHERY_MANAGER_HWMOCK_H

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

#endif //PERIPHERY_MANAGER_HWMOCK_H
