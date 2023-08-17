#include <iostream>
#include "TemperatureSensorProtocol.h"

std::vector<uint8_t> TemperatureSensorProtocol::serialize(const std::vector<uint8_t> &tx_data) {
    auto tx_packet = Packet(tx_data);
    std::cout << "TX Packet: " << tx_packet << std::endl;

    return tx_packet.toVector();
}

std::vector<uint8_t> TemperatureSensorProtocol::deserialize(const std::vector<uint8_t>& rx_packet) {
    if (rx_packet.size() < 3) {
        throw std::invalid_argument("Packet too small to be valid");
    }

    Packet packet;
    auto it = rx_packet.begin();

    packet.header = *it++;
    packet.size = *it++;
    packet.data.assign(it, it + packet.size);
    it += packet.size;

    if (rx_packet.end() - it < 1) {
        throw std::invalid_argument("Packet too small for checksum");
    }
    packet.checksum = *it;

    // Validate checksum
    uint8_t calculated_checksum = 0;
    for (const auto& byte : packet.data) {
        calculated_checksum += byte;
    }
    if (calculated_checksum != packet.checksum) {
        throw std::invalid_argument("Checksum doesn't match, packet might be corrupted");
    }

    std::cout << "RX Packet: " << packet << std::endl;

    return packet.data;
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
