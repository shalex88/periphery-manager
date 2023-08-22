#ifndef PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
#define PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H

#include "../PeripheryManager/ProtocolInterface.h"
#include <ostream>

class TemperatureSensorProtocol : public ProtocolInterface {
public:
    struct Packet {
        uint8_t header;
        uint8_t size;
        std::vector<uint8_t> data;
        uint8_t checksum;

        Packet() : header(0), size(0), checksum(0) {};
        explicit Packet(const std::vector<uint8_t>& data) : header('$'), size(static_cast<uint8_t>(data.size())), data(data), checksum(0) {
            for (const auto& byte : data) {
                checksum += byte;
            }
        };

        std::vector<uint8_t> toVector() const {
            std::vector<uint8_t> result;

            result.push_back(header);
            result.push_back(size);
            result.insert(result.end(), data.begin(), data.end());
            result.push_back(checksum);

            return result;
        }

        friend std::ostream& operator<<(std::ostream& os, const Packet& packet);
    };

    TemperatureSensorProtocol() = default;
    ~TemperatureSensorProtocol() override = default;
    std::vector<uint8_t> serialize(const std::vector<uint8_t>& tx_data) override;
    std::vector<uint8_t> deserialize(const std::vector<uint8_t>& rx_packet) override;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSORPROTOCOL_H
