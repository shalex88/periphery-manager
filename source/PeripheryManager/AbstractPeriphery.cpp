#include "AbstractPeriphery.h"
#include <iostream>

std::vector<uint8_t> AbstractPeriphery::readData() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto rx_packet = communication_interface_->read();
    auto data = protocol_interface_->unpackData(rx_packet);

    return data;
}

bool AbstractPeriphery::writeData(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto tx_packet = protocol_interface_->packData(tx_data);

    return tx_packet.size() == communication_interface_->write(tx_packet);
}

std::vector<uint8_t> AbstractPeriphery::getDataSyncronously(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::vector<uint8_t> respose_rx;

    if(writeData(tx_data)) {
        respose_rx = readData();
    } else {
        throw std::runtime_error("Not all data was written");
    }

    return respose_rx;
}
