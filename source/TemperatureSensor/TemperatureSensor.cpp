#include <unordered_map>
#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensor.h"

enum class COMMAND {
    GET_STATUS,
    GET_TEMPERATURE,
    GET_HUMIDITY
};

std::unordered_map<COMMAND, std::vector<uint8_t>> command = {
    {COMMAND::GET_STATUS, {1}},
    {COMMAND::GET_TEMPERATURE, {25}},
    {COMMAND::GET_HUMIDITY, {1, 1}}
};

uint8_t TemperatureSensor::init() {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    initCommunication();

    return 0;
}

uint8_t TemperatureSensor::deinit() {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    //TODO: implement

    return 0;
}

uint8_t TemperatureSensor::getStatus() {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    auto status = getDataSyncronously(command[COMMAND::GET_STATUS]).at(0);

    return status;
}

uint8_t TemperatureSensor::getTemperature() {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    auto temperature = getDataSyncronously(command[COMMAND::GET_TEMPERATURE]).at(0);

    return temperature;
}

uint16_t TemperatureSensor::getHumidity() {
    LOG_TRACE("{}", __PRETTY_FUNCTION__);

    auto rx_data = getDataSyncronously(command[COMMAND::GET_HUMIDITY]);

    uint16_t humidity = (static_cast<uint16_t>(rx_data[0]) << 8) | static_cast<uint16_t>(rx_data[1]);

    return humidity;
}

std::future<uint8_t> TemperatureSensor::getTemperatureAsynchronously() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::promise<uint8_t> prom;
    auto future_result = prom.get_future();

    auto rx_data_future = getDataAsynchronously(command[COMMAND::GET_TEMPERATURE]);

    auto temperature = rx_data_future.get().at(0);

    prom.set_value(temperature);

    return future_result;
}
