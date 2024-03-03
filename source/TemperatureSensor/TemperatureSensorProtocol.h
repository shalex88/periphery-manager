#ifndef PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
#define PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H

#include <ostream>
#include "PeripheryManager/ProtocolInterface.h"

class TemperatureSensorProtocol : public ProtocolInterface {
public:
    TemperatureSensorProtocol() = default;
    ~TemperatureSensorProtocol() override = default;
    std::vector<uint8_t> packData(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> unpackData(const std::vector<uint8_t>& packet) override;
private:
    struct Packet {
        uint8_t header;
        uint8_t size;
        std::vector<uint8_t> data;
        uint8_t checksum;
    };
    Packet serialize(const std::vector<uint8_t>& packet);
    std::vector<uint8_t> deserialize(const Packet& packet);
    std::string packetToString(const Packet& packet) const;
    uint8_t calculateChecksum(const std::vector<uint8_t> &data);
    bool verifyChecksum(Packet &packet);
    void logPackage(const std::string& message, const TemperatureSensorProtocol::Packet &serialized_packet) const;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
