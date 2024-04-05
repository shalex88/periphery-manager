#include <thread>
#include <iostream>
#include "Logger/Logger.h"
#include "PeripheryManager/AbstractDevice.h"

std::vector<uint8_t> AbstractDevice::readData() {
    auto rx_packet = communication_interface_->read();
    auto data = protocol_interface_->unpackData(rx_packet);

    return data;
}

bool AbstractDevice::writeData(const std::vector<uint8_t> &tx_data) {
    auto tx_packet = protocol_interface_->packData(tx_data);

    return tx_packet.size() == communication_interface_->write(tx_packet);
}

std::vector<uint8_t> AbstractDevice::getDataSyncronously(const std::vector<uint8_t> &tx_data) {
    assertIfDeviceIsDisabled();

    std::vector<uint8_t> respose_rx;

    if (writeData(tx_data)) {
        respose_rx = readData();
    } else {
        LOG_ERROR("{}", "Not all data was written");
        throw std::runtime_error("Not all data was written");
    }

    return respose_rx;
}

void AbstractDevice::assertIfDeviceIsDisabled() const {
    //TODO: find better way than exception, don't stop execution
    if(!is_enabled_) {
        LOG_ERROR("{}", "Device is disabled");
        throw std::runtime_error("Device is disabled");
    }
}

std::future<std::vector<uint8_t>> AbstractDevice::getDataAsynchronously(const std::vector<uint8_t> &tx_data) {
    assertIfDeviceIsDisabled();

    std::promise<std::vector<uint8_t>> prom;
    auto future_result = prom.get_future();

    if(writeData(tx_data)) {
        // Simulate or implement actual asynchronous operation for reading data
        std::thread([prom = std::move(prom), this]() mutable {
            // Wait until the data is ready (replace with your actual logic)
            std::this_thread::sleep_for(std::chrono::seconds(1)); //FIXME: use a mechanism to check if data received
            std::vector<uint8_t> rx_data = readData();
            prom.set_value(rx_data);
        }).detach();
    } else {
        throw std::runtime_error("Not all data was written");
    }

    return future_result;
}

bool AbstractDevice::init() {
    if (enable()) {
        if (!communication_interface_->init()) {
            LOG_ERROR("Failed to connect");
            disable();
            return false;
        }
    } else {
        LOG_ERROR("Failed to turn on");
        return false;
    }

    return is_enabled_ = true;
}

bool AbstractDevice::deinit() {
    if (is_enabled_) {
        if (communication_interface_->deinit()) {
            if (!disable()) {
                LOG_ERROR("Failed to turn off");
                return false;
            }
        } else {
            LOG_ERROR("Failed to disconnect");
            return false;
        }
    }

    return true;
}
