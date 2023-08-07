#include "AbstractPeriphery.h"

std::vector<uint8_t> AbstractPeriphery::readData() {
    return interface_->read();
}

uint8_t AbstractPeriphery::writeData(std::vector<uint8_t> &tx_data) {
    return interface_->write(tx_data);
}
