#include "PeripheryManager/CommunicationMock.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"
#include "Logger/Logger.h"

int main() {
    LOG_INFO("periphery-manager {}.{}.{}", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);

    auto communication_interface = std::make_shared<CommunicationMock>();
    auto protocol_interface = std::make_shared<TemperatureSensorProtocol>();
    auto temp_sensor = std::make_shared<TemperatureSensor>(communication_interface, protocol_interface);

    temp_sensor->init();

    LOG_INFO("{}", temp_sensor->getStatus());
    LOG_INFO("{}", temp_sensor->getTemperature());
    LOG_INFO("{}", temp_sensor->getHumidity());

    temp_sensor->deinit();

    spdlog::drop_all();

    return 0;
}