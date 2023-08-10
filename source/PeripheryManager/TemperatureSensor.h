#ifndef PERIPHERY_MANAGER_TEMPERATURESENSOR_H
#define PERIPHERY_MANAGER_TEMPERATURESENSOR_H

#include "AbstractPeriphery.h"
#include "CommunicationInterface.h"
#include <memory>
#include <utility>

class TemperatureSensor : public AbstractPeriphery {
public:
    explicit TemperatureSensor(std::shared_ptr<CommunicationInterface> interface) : AbstractPeriphery(std::move(interface)) {};
    ~TemperatureSensor() override = default;
    uint8_t init() override;
    uint8_t deinit() override;
    uint8_t getStatus() override;
    uint8_t getTemperature();
};

#endif //PERIPHERY_MANAGER_TEMPERATURESENSOR_H
