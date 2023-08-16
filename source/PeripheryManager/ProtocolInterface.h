#ifndef PERIPHERY_MANAGER_PROTOCOLINTERFACE_H
#define PERIPHERY_MANAGER_PROTOCOLINTERFACE_H

#include <vector>
#include <cstdint>

class ProtocolInterface {
public:
    virtual ~ProtocolInterface() = default;
    virtual std::vector<uint8_t> serialize(const std::vector<uint8_t>& tx_data) = 0;
    virtual std::vector<uint8_t> deserialize(const std::vector<uint8_t>& rx_packet) = 0;
};

#endif //PERIPHERY_MANAGER_PROTOCOLINTERFACE_H
