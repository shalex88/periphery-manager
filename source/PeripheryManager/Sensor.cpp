#include "Sensor.h"

uint8_t Sensor::init() {
    // TODO: implement
    return 0;
}

uint8_t Sensor::deinit() {
    // TODO: implement
    return 0;
}

uint8_t Sensor::getStatus() {
    auto get_status_cmd = std::vector<uint8_t>{0, 1, 2};
    if(writeData(get_status_cmd) == get_status_cmd.size()) {
        auto respose_rx = readData();
        // TODO: parse data
    }

    return 0;
}

uint8_t Sensor::getTemperature() {
    auto get_temperature_cmd = std::vector<uint8_t>{0, 1, 2};
    if(writeData(get_temperature_cmd) == get_temperature_cmd.size()) {
        auto respose_rx = readData();
        // TODO: parse data
    }

    return 25;
}
