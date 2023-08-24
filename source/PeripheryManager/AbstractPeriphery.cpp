#include "AbstractPeriphery.h"
#include <iostream>

std::vector<uint8_t> AbstractPeriphery::readData() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto rx_package = communication_interface_->read();
    auto data = protocol_interface_->unpackData(rx_package);

    return data;
}

uint8_t AbstractPeriphery::writeData(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto tx_package = protocol_interface_->packData(tx_data);

    return communication_interface_->write(tx_package);
}

std::vector<uint8_t> AbstractPeriphery::getDataSyncronously(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::vector<uint8_t> respose_rx;

    if(writeData(tx_data) > tx_data.size()) { //TODO: how to retceive a serialized packet size?
        respose_rx = readData();
    } else {
        throw std::runtime_error("Not all data was written");
    }

    return respose_rx;
}
