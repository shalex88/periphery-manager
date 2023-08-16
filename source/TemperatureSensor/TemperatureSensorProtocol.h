#ifndef PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
#define PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H

#include "../PeripheryManager/ProtocolInterface.h"

class TemperatureSensorProtocol : public ProtocolInterface {
public:
    TemperatureSensorProtocol() = default;
    ~TemperatureSensorProtocol() override = default;
    std::vector<uint8_t> serialize(const std::vector<uint8_t>& tx_data) override;
    std::vector<uint8_t> deserialize(const std::vector<uint8_t>& rx_packet) override;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
