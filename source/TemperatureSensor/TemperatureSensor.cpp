#include "TemperatureSensor.h"
#include <unordered_map>
#include <optional>
#include "Logger/Logger.h"

enum class DeviceCommand {
    GetStatus,
    GetTemperature,
    GetHumidity
};

std::unordered_map<DeviceCommand, std::vector<uint8_t>> DeviceCommandsToBytes = {
    {DeviceCommand::GetStatus, {1}},
    {DeviceCommand::GetTemperature, {25}},
    {DeviceCommand::GetHumidity, {1, 1}}
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

std::optional<uint8_t> TemperatureSensor::getStatus() const {
    auto status = getDataSyncronously(DeviceCommandsToBytes[DeviceCommand::GetStatus]);

    if (!status.has_value()) {
        return std::nullopt;
    }

    return status.value().at(0);
}

std::optional<uint8_t> TemperatureSensor::getTemperature() const {
    auto temperature = getDataSyncronously(DeviceCommandsToBytes[DeviceCommand::GetTemperature]);

    if (!temperature.has_value()) {
        return std::nullopt;
    }

    return temperature.value().at(0);
}

std::optional<uint16_t> TemperatureSensor::getHumidity() const {
    auto humidity = getDataSyncronously(DeviceCommandsToBytes[DeviceCommand::GetHumidity]);

    if (!humidity.has_value()) {
        return std::nullopt;
    }

    return (static_cast<uint16_t>(humidity.value()[0]) << 8) | static_cast<uint16_t>(humidity.value()[1]);
}

uint8_t TemperatureSensor::getTemperatureAsynchronously() const {
    std::promise<uint8_t> prom;
    auto future_result = prom.get_future();

    auto rx_data_future = getDataAsynchronously(DeviceCommandsToBytes[DeviceCommand::GetTemperature]);
    auto temperature = rx_data_future.get().at(0);
    prom.set_value(temperature);

    return future_result.get();
}
