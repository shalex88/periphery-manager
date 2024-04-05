#include <unordered_map>
#include <optional>
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

TemperatureSensor::~TemperatureSensor() {
    deinit();
}

bool TemperatureSensor::enable() {
    LOG_INFO("Turn On Temperature Sensor");

    return true;
}

bool TemperatureSensor::disable() {
    LOG_INFO("Turn Off Temperature Sensor");

    return true;
}

std::optional<uint8_t> TemperatureSensor::getStatus() {
    auto status = getDataSyncronously(command[Command::kGetStatus]);

    if (status.has_value()) {
        return status.value().at(0);
    } else {
        return std::nullopt;
    }
}

std::optional<uint8_t> TemperatureSensor::getTemperature() {
    auto temperature = getDataSyncronously(command[Command::kGetTemperature]);

    if (temperature.has_value()) {
        return temperature.value().at(0);
    } else {
        return std::nullopt;
    }
}

std::optional<uint16_t> TemperatureSensor::getHumidity() {
    auto humidity = getDataSyncronously(command[Command::kGetHumidity]);

    if (humidity.has_value()) {
        return (static_cast<uint16_t>(humidity.value()[0]) << 8) | static_cast<uint16_t>(humidity.value()[1]);
    } else {
        return std::nullopt;
    }
}

uint8_t TemperatureSensor::getTemperatureAsynchronously() {
    std::promise<uint8_t> prom;
    auto future_result = prom.get_future();

    auto rx_data_future = getDataAsynchronously(command[Command::kGetTemperature]);

    auto temperature = rx_data_future.get().at(0);

    prom.set_value(temperature);

    return temperature;
}
