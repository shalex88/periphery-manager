#include "TemperatureSensorProtocol.h"

std::vector<uint8_t> TemperatureSensorProtocol::serialize(const std::vector<uint8_t> &tx_data) {
    std::vector<uint8_t> tx_packet;

    tx_packet.push_back('$');
    tx_packet.push_back(static_cast<uint8_t>(tx_data.size()));

    uint8_t checksum{};
    for(const auto & data : tx_data) {
        tx_packet.push_back(data);
        checksum += data;
    }

    tx_packet.push_back(checksum);

    return tx_packet;
}

std::vector<uint8_t> TemperatureSensorProtocol::deserialize(const std::vector<uint8_t> &rx_packet) {
    std::vector<uint8_t> rx_data;

    // Check if packet size is > 2 ($ and size included)
    if(rx_packet.size() < 3) {
        return rx_data;
    }

    // Check if packet starts with '$'
    if(rx_packet[0] != '$') {
        return rx_data;
    }

    // Extract data size
    uint8_t data_size = rx_packet[1];

    // Check if data size matches the packet
    if(data_size + 3 != rx_packet.size()) {
        return rx_data;
    }

    // Calculate checksum
    uint8_t checksum = 0;
    for(std::size_t i = 2; i < data_size + 2; ++i) {
        checksum += rx_packet[i];
    }

    // Check if checksum matches
    if(checksum != rx_packet.back()) {
        return rx_data;
    }

    // Extract data
    rx_data.assign(rx_packet.begin() + 2, rx_packet.end() - 1);

    return rx_data;
}