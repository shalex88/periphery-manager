#include "PeripheryManager/CommunicationMock.h"
#include "PeripheryManager/CommunicationTcpClient.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"
#include "Logger/Logger.h"

int main() {
    LOG_INFO("periphery-manager {}.{}.{}", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);

    auto communication_interface = std::make_shared<CommunicationMock>();
//    auto communication_interface = std::make_shared<CommunicationTcpClient>("127.0.0.1", 8080);
    auto protocol_interface = std::make_shared<TemperatureSensorProtocol>();
    auto temp_sensor = std::make_shared<TemperatureSensor>(communication_interface, protocol_interface);

    if (temp_sensor->init()) {
        LOG_INFO("{}", temp_sensor->getStatus());
        LOG_INFO("{}", temp_sensor->getTemperature());
        LOG_INFO("{}", temp_sensor->getHumidity());
        LOG_INFO("{}", temp_sensor->getTemperatureAsynchronously());
        temp_sensor->deinit();
    }

    return 0;
}