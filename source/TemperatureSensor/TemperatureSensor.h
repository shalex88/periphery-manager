#ifndef PERIPHERY_MANAGER_TEMPERATURESENSOR_H
#define PERIPHERY_MANAGER_TEMPERATURESENSOR_H

#include <future>
#include "PeripheryManager/AbstractDevice.h"

class TemperatureSensor : public AbstractDevice {
public:
    TemperatureSensor(std::shared_ptr<HwInterface> hw_interface,
                               std::shared_ptr<ProtocolInterface> protocol_interface) :
            AbstractDevice(std::move(hw_interface), std::move(protocol_interface)) {};
    ~TemperatureSensor() override = default;
    uint8_t getStatus() override;
    uint8_t getTemperature();
    uint16_t getHumidity();
    uint8_t getTemperatureAsynchronously();
private:
    bool enable() override;
    bool disable() override;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSOR_H
