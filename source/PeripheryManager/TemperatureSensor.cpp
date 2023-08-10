#include "TemperatureSensor.h"
#include <iostream>
#include <unordered_map>

enum class COMMAND {
    GET_STATUS,
    GET_TEMPERATURE
};

std::unordered_map<COMMAND, std::vector<uint8_t>> command = {
        {COMMAND::GET_STATUS, {0, 1, 2}},
        {COMMAND::GET_TEMPERATURE, {0, 25, 2}}
};

uint8_t TemperatureSensor::init() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    uint8_t result{1};

    if (getStatus() != 0xFF) {
        result = 0;
    }

    return result;
}

uint8_t TemperatureSensor::deinit() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    uint8_t result{1};

    if (getStatus() == 0xFF) {
        result = 0;
    }

    return result;
}

uint8_t TemperatureSensor::getStatus() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return getDataSyncroniously(command[COMMAND::GET_STATUS]);
}

uint8_t TemperatureSensor::getTemperature() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return getDataSyncroniously(command[COMMAND::GET_TEMPERATURE]);
}
