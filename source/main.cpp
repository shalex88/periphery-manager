#include <iostream>
#include "PeripheryManager/CommunicationMock.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"

int main() {
    std::cout << "periphery-manager" << " " << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << "." << APP_VERSION_PATCH << std::endl;

    auto communication_interface = std::make_shared<CommunicationMock>();
    auto protocol_interface = std::make_shared<TemperatureSensorProtocol>();
    auto temp_sensor = std::make_shared<TemperatureSensor>(communication_interface, protocol_interface);

    temp_sensor->init();

    std::cout << static_cast<int>(temp_sensor->getStatus()) << std::endl;
    std::cout << static_cast<int>(temp_sensor->getTemperature()) << std::endl;
    std::cout << static_cast<int>(temp_sensor->getHumidity()) << std::endl;

    temp_sensor->deinit();

    return 0;
}