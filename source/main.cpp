#include <iostream>
#include "PeripheryManager/HwMock.h"
#include "TemperatureSensor/TemperatureSensor.h"
#include "TemperatureSensor/TemperatureSensorProtocol.h"
#include "Camera/DayCamera/DayCamera.h"
#include "Camera/DayCamera/DayCameraProtocol.h"

int main() {
    std::cout << "periphery-manager" << " " << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << "." << APP_VERSION_PATCH << std::endl;

    std::cout << "" << std::endl;
    std::cout << "Temperature Sensor" << std::endl;
    auto temp_sensor_hw = std::make_shared<HwMock>();
    auto temp_sensor_protocol = std::make_shared<TemperatureSensorProtocol>();
    auto temp_sensor = std::make_shared<TemperatureSensor>(temp_sensor_hw, temp_sensor_protocol);

    temp_sensor->init();

    std::cout << static_cast<int>(temp_sensor->getStatus()) << std::endl;
    std::cout << static_cast<int>(temp_sensor->getTemperature()) << std::endl;
    std::cout << static_cast<int>(temp_sensor->getHumidity()) << std::endl;

    temp_sensor->deinit();

    std::cout << "" << std::endl;
    std::cout << "Day Camera" << std::endl;
    auto day_cam_hw = std::make_shared<HwMock>();
    auto day_cam_protocol = std::make_shared<DayCameraProtocol>();
    auto day_cam = std::make_shared<DayCamera>(day_cam_hw, day_cam_protocol);

    day_cam->init();
    day_cam->getZoom();
    day_cam->zoomIn();
    day_cam->zoomIn();
    day_cam->zoomOut();

    std::cout << static_cast<int>(day_cam->getStatus()) << std::endl;

    day_cam->deinit();
/*

    auto thermal_cam_hw = std::make_shared<HwMock>();
    auto thermal_protocol = std::make_shared<TemperatureSensorProtocol>();
    auto thermal_cam = std::make_shared<TemperatureSensor>(day_cam_hw, day_cam_protocol);

    thermal_cam->init();
    thermal_cam->zoom_in();
    thermal_cam->zoom_out();

    std::cout << static_cast<int>(thermal_cam->getStatus()) << std::endl;

    thermal_cam->deinit();
*/

    return 0;
}