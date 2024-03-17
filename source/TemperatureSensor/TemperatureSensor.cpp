#include <unordered_map>
#include "Logger/Logger.h"
#include "TemperatureSensor/TemperatureSensor.h"

enum class Command {
    kGetStatus,
    kGetTemperature,
    kGetHumidity
};

std::unordered_map<Command, std::vector<uint8_t>> command = {
    {Command::kGetStatus,      {1}},
    {Command::kGetTemperature, {25}},
    {Command::kGetHumidity,    {1, 1}}
};

bool TemperatureSensor::enable() {
    LOG_INFO("Turn On Temperature Sensor");

    return true;
}

bool TemperatureSensor::disable() {
    LOG_INFO("Turn Off Temperature Sensor");

    return true;
}

uint8_t TemperatureSensor::getStatus() {
    auto status = getDataSyncronously(command[Command::kGetStatus]).at(0);

    return status;
}

uint8_t TemperatureSensor::getTemperature() {
    auto temperature = getDataSyncronously(command[Command::kGetTemperature]).at(0);

    return temperature;
}

uint16_t TemperatureSensor::getHumidity() {
    auto rx_data = getDataSyncronously(command[Command::kGetHumidity]);

    uint16_t humidity = (static_cast<uint16_t>(rx_data[0]) << 8) | static_cast<uint16_t>(rx_data[1]);

    return humidity;
}

uint8_t TemperatureSensor::getTemperatureAsynchronously() {
    std::promise<uint8_t> prom;
    auto future_result = prom.get_future();

    auto rx_data_future = getDataAsynchronously(command[Command::kGetTemperature]);

    auto temperature = rx_data_future.get().at(0);

    prom.set_value(temperature);

    return temperature;
}
