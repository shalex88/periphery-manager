#ifndef PERIPHERY_MANAGER_SENSOR_H
#define PERIPHERY_MANAGER_SENSOR_H

#include "AbstractPeriphery.h"
#include "CommunicationInterface.h"
#include <memory>
#include <utility>

class Sensor : public AbstractPeriphery {
public:
    explicit Sensor(std::shared_ptr<CommunicationInterface> interface) : AbstractPeriphery(std::move(interface)) {};
    ~Sensor() override = default;
    uint8_t init() override;
    uint8_t deinit() override;
    uint8_t getStatus() override;
    uint8_t getTemperature();
};

#endif //PERIPHERY_MANAGER_SENSOR_H
