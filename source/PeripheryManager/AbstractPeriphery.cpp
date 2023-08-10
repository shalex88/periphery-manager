#include "AbstractPeriphery.h"
#include <iostream>

std::vector<uint8_t> AbstractPeriphery::readData() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return interface_->read();
}

uint8_t AbstractPeriphery::writeData(std::vector<uint8_t> &tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return interface_->write(tx_data);
}

uint8_t AbstractPeriphery::getDataSyncroniously(std::vector<uint8_t>& tx_data) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if(writeData(tx_data) == tx_data.size()) {
        auto respose_rx = readData();

        if(!respose_rx.empty()) {
            auto data = respose_rx.at(1);
            return data;
        }
    }

    return 0xff;
}
