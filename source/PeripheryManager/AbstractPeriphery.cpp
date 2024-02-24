#include "AbstractPeriphery.h"
#include <iostream>

std::vector<uint8_t> AbstractPeriphery::readData() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto rx_packet = hw_interface_->read();
    auto data = protocol_interface_->unpackData(rx_packet);

    return data;
}

bool AbstractPeriphery::writeData(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto tx_packet = protocol_interface_->packData(tx_data);

    return tx_packet.size() == hw_interface_->write(tx_packet);
}

std::vector<uint8_t> AbstractPeriphery::getDataSyncronously(const std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::vector<uint8_t> respose_rx;

    if(writeData(tx_data)) {
        respose_rx = readData();
    } else {
        throw std::runtime_error("Not all data was written");
    }

    //TODO: check the command id or not?
    /*
    if(respose_rx.at(0) != tx_data.at(0)) {
        throw std::runtime_error("Response command doesn't fit the request");
    }
    */

    //TODO: strip the command id or not? If yes is this function a good place?
    /*
    std::vector<uint8_t> result(respose_rx.begin() + 1, respose_rx.end());
    return result;
    */

    return respose_rx;
}
