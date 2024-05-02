#ifndef PERIPHERY_MANAGER_TEMPERATURESENSOR_H
#define PERIPHERY_MANAGER_TEMPERATURESENSOR_H

#include "PeripheryManager/AbstractDevice.h"

class TemperatureSensor : public AbstractDevice {
public:
    TemperatureSensor(std::shared_ptr<HwInterface> hw_interface,
                               std::shared_ptr<ProtocolInterface> protocol_interface) :
            AbstractDevice(std::move(hw_interface), std::move(protocol_interface)) {};
    ~TemperatureSensor() override;
    std::optional<uint8_t> getStatus() const;
    std::optional<uint8_t> getTemperature() const;
    std::optional<uint16_t> getHumidity() const;
    uint8_t getTemperatureAsynchronously() const;
private:
    bool enable() override;
    bool disable() override;
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSOR_H
