#ifndef PERIPHERY_MANAGER_TEMPERATURESENSOR_H
#define PERIPHERY_MANAGER_TEMPERATURESENSOR_H

#include "PeripheryManager/AbstractPeriphery.h"

class TemperatureSensor : public AbstractPeriphery {
public:
    TemperatureSensor(std::shared_ptr<CommunicationInterface> communication_interface,
                               std::shared_ptr<ProtocolInterface> protocol_interface) :
                               AbstractPeriphery(std::move(communication_interface), std::move(protocol_interface)) {};
    ~TemperatureSensor() override = default;
    uint8_t init() override;
    uint8_t deinit() override;
    uint8_t getStatus() override;
    uint8_t getTemperature();
    uint16_t getHumidity();
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSOR_H
