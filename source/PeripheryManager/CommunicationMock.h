#ifndef PERIPHERY_MANAGER_COMMUNICATIONMOCK_H
#define PERIPHERY_MANAGER_COMMUNICATIONMOCK_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "Logger/Logger.h"
#include "PeripheryManager/CommunicationInterface.h"

class CommunicationMock : public CommunicationInterface {
public:
    CommunicationMock() = default;
    ~CommunicationMock() override = default;
    std::vector<uint8_t> read() override;
    uint8_t write(const std::vector<uint8_t>& tx_data) override;
private:
    std::vector<uint8_t> last_tx_data_;
};

std::vector<uint8_t> CommunicationMock::read() {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    return last_tx_data_;
}

uint8_t CommunicationMock::write(const std::vector<uint8_t> &tx_data) {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    last_tx_data_ = tx_data;

    return tx_data.size();
}

#endif //PERIPHERY_MANAGER_COMMUNICATIONMOCK_H
