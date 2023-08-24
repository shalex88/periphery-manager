#include <iostream>
#include "TemperatureSensorProtocol.h"

std::vector<uint8_t> TemperatureSensorProtocol::serialize(const std::vector<uint8_t> &tx_data) {
    auto tx_packet = Packet(tx_data);

    std::cout << "TX Packet: " << tx_packet << std::endl;

    return tx_packet.toVector();
}

std::vector<uint8_t> TemperatureSensorProtocol::deserialize(const std::vector<uint8_t>& rx_packet) {
    const auto k_minimal_packet_size = sizeof(Packet::header) + sizeof(Packet::size)
            + sizeof(uint8_t) + sizeof(Packet::checksum); //TODO: Check somehow the data element size instead uint8_t
    if (rx_packet.size() < k_minimal_packet_size) {
        throw std::runtime_error("Packet too small to be valid");
    }

    Packet packet;
    auto it = rx_packet.begin();

    packet.header = *it++;
    packet.size = *it++;
    packet.data.assign(it, it + packet.size);
    it += packet.size;

    packet.checksum = *it;

    if (verifyChecksum(packet)) {
        std::cout << "RX Packet: " << packet << std::endl;
        return packet.data;
    } else {
        throw std::runtime_error("Checksum doesn't match, packet might be corrupted");
    }
}

bool TemperatureSensorProtocol::verifyChecksum(TemperatureSensorProtocol::Packet &packet) {
    uint8_t calculated_checksum = 0;

    for (const auto& byte : packet.data) {
        calculated_checksum += byte;
    }

    return calculated_checksum == packet.checksum;
}

std::ostream& operator<<(std::ostream &os, const TemperatureSensorProtocol::Packet &packet) {
    os << "Header: " << static_cast<char>(packet.header);
    os << " Size: " << static_cast<int>(packet.size);
    os << " Data: ";

    for (auto byte: packet.data) {
        os << static_cast<int>(byte) << " ";
    }

    os << " Checksum: " << static_cast<int>(packet.checksum);

    return os;
}
