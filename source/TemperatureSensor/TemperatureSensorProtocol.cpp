#include <iostream>
#include <sstream>
#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"

std::vector<uint8_t> TemperatureSensorProtocol::packData(const std::vector<uint8_t> &data) {
    Packet serialized_packet;

    serialized_packet.header = '$';
    serialized_packet.size = static_cast<uint8_t>(data.size());
    serialized_packet.data = data;
    serialized_packet.checksum = calculateChecksum(data);

    logPackage("TX", serialized_packet);

    return deserialize(serialized_packet);
}

void TemperatureSensorProtocol::logPackage(const std::string& message, const TemperatureSensorProtocol::Packet &serialized_packet) const {
//    std::cout << message << " " << packetToString(serialized_packet) << std::endl;
    LOG_DEBUG("{} {}", message, packetToString(serialized_packet));
}

std::vector<uint8_t> TemperatureSensorProtocol::unpackData(const std::vector<uint8_t> &packet) {
    auto serialized_packet = serialize(packet);

    return serialized_packet.data;
}

uint8_t TemperatureSensorProtocol::calculateChecksum(const std::vector<uint8_t>& data) {
    uint8_t checksum = 0;

    for (const auto &byte: data) {
        checksum += byte;
    }

    return checksum;
}

bool TemperatureSensorProtocol::verifyChecksum(TemperatureSensorProtocol::Packet &packet) {
    return calculateChecksum(packet.data) == packet.checksum;
}

TemperatureSensorProtocol::Packet TemperatureSensorProtocol::serialize(const std::vector<uint8_t> &packet) {
    const size_t k_minimal_packet_size = sizeof(Packet::header) + sizeof(Packet::size)
                                       + sizeof(uint8_t) + sizeof(Packet::checksum); //TODO: Check somehow the data element size instead uint8_t
    if (packet.size() < k_minimal_packet_size) {
        throw std::runtime_error("Packet too small to be valid");
    }

    Packet serialized_packet;
    auto it = packet.begin();

    serialized_packet.header = *it++;
    serialized_packet.size = *it++;
    serialized_packet.data.assign(it, it + serialized_packet.size);
    it += serialized_packet.size;

    serialized_packet.checksum = *it;

    if (!verifyChecksum(serialized_packet)) {
        throw std::runtime_error("Checksum doesn't match, packet might be corrupted");
    }

    logPackage("RX", serialized_packet);

    return serialized_packet;
}

std::vector<uint8_t> TemperatureSensorProtocol::deserialize(const Packet &packet) {
    std::vector<uint8_t> deserialized_packet;

    deserialized_packet.push_back(packet.header);
    deserialized_packet.push_back(packet.size);
    deserialized_packet.insert(deserialized_packet.end(), packet.data.begin(), packet.data.end());
    deserialized_packet.push_back(packet.checksum);

    return deserialized_packet;
}

std::string TemperatureSensorProtocol::packetToString(const TemperatureSensorProtocol::Packet &packet) const {
    std::ostringstream os;
    os << "Header: " << static_cast<char>(packet.header);
    os << " Size: " << static_cast<int>(packet.size);
    os << " Data: ";
    for (auto byte: packet.data) {
        os << static_cast<int>(byte) << " ";
    }
    os << " Checksum: " << static_cast<int>(packet.checksum);
    return os.str();
}
