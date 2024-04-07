#include <csignal>
#include "AppInputs/TcpMessageServer.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"
#include "PeripheryManager/HwMock.h"
#include "App/SignalHandler.h"
#include "App.h"

std::atomic<bool> App::keep_running_ = true;

void App::shutdown() {
    keep_running_ = false;
    LOG_INFO("Terminating...");
}

void App::run() {
    SignalHandler::setupSignalHandling();

    auto hw_interface = std::make_shared<HwMock>();
//    auto hw_interface = std::make_shared<TcpClient>("127.0.0.1", 12345);
    auto protocol_interface = std::make_shared<TemperatureSensorProtocol>();
    auto temp_sensor = std::make_shared<TemperatureSensor>(hw_interface, protocol_interface);
    temp_sensor->init();

    uint32_t cores_num = sysconf(_SC_NPROCESSORS_ONLN);
    LOG_INFO("CPU cores number is {}", cores_num);
    auto scheduler = std::make_shared<Scheduler>(cores_num);
    scheduler->init();

    auto dispatcher = std::make_shared<CommandDispatcher>(scheduler);
    dispatcher->registerCommand("stop", std::make_shared<StopCommand>());
    dispatcher->registerCommand("temp", std::make_shared<GetTempCommand>(temp_sensor));

    const int tcp_server_port = 12345;
    auto tcp_server = std::make_shared<TcpMessageServer>(tcp_server_port, dispatcher);
    tcp_server->init();

    while(keep_running_) {
        //TODO: bit
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG_DEBUG("Main thread stopped");
}
