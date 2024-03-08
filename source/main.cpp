#include "PeripheryManager/HwMock.h"
//#include "PeripheryManager/TcpClient.h"
#include "TcpServer/TcpServer.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"
#include "Logger/Logger.h"

int main() {
    LOG_INFO("periphery-manager {}.{}.{}", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);

    auto hw_interface = std::make_shared<HwMock>();
//    auto hw_interface = std::make_shared<TcpClient>("127.0.0.1", 12345);
    auto protocol_interface = std::make_shared<TemperatureSensorProtocol>();
    auto temp_sensor = std::make_shared<TemperatureSensor>(hw_interface, protocol_interface);

    // Default severity level is Info
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kInfo);

    if (temp_sensor->init()) {
        LOG_INFO("{}", temp_sensor->getStatus());
        LOG_INFO("{}", temp_sensor->getTemperature());
        LOG_INFO("{}", temp_sensor->getHumidity());
        LOG_INFO("{}", temp_sensor->getTemperatureAsynchronously());
        temp_sensor->deinit();
    }

    int tcp_server_port = 12345;
    auto tcp_server = std::make_shared<TcpServer>(tcp_server_port);
    tcp_server->init();

    while(true) {

    }

    return 0;
}