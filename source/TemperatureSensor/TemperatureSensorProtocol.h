#ifndef PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
#define PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H

#include <ostream>
#include "PeripheryManager/ProtocolInterface.h"

class TemperatureSensorProtocol : public ProtocolInterface {
public:
    TemperatureSensorProtocol() = default;
    ~TemperatureSensorProtocol() override = default;
    std::vector<uint8_t> packData(const std::vector<uint8_t>& data) const override;
    std::vector<uint8_t> unpackData(const std::vector<uint8_t>& packet) const override;
private:
    struct Packet;
    Packet serialize(const std::vector<uint8_t>& packet) const;
    std::vector<uint8_t> deserialize(const Packet& packet) const;
    std::string packetToString(const Packet& packet) const;
    uint8_t calculateChecksum(const std::vector<uint8_t> &data) const;
    bool verifyChecksum(Packet &packet) const;
    void logPackage(const std::string& message, const TemperatureSensorProtocol::Packet& serialized_packet) const;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
