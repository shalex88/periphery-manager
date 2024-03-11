#include <csignal>
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
    SET_LOG_LEVEL(LoggerInterface::LogLevel::kTrace);

    if (temp_sensor->init()) {
        LOG_DEBUG("{}", temp_sensor->getStatus());
        LOG_DEBUG("{}", temp_sensor->getTemperature());
        LOG_DEBUG("{}", temp_sensor->getHumidity());
        LOG_DEBUG("{}", temp_sensor->getTemperatureAsynchronously());
        temp_sensor->deinit();
    }

    uint32_t cores_num = sysconf(_SC_NPROCESSORS_ONLN);
    LOG_INFO("CPU cores number is {}", cores_num);
    auto scheduler = std::make_shared<Scheduler>(cores_num);
    int tcp_server_port = 12345;
    auto tcp_server = std::make_shared<TcpServer>(tcp_server_port, scheduler);
    tcp_server->init();

    while(true) {

    }

    return 0;
}