#ifndef PERIPHERY_MANAGER_DAYCAMERA_H
#define PERIPHERY_MANAGER_DAYCAMERA_H

#include "../../PeripheryManager/AbstractPeriphery.h"

class DayCamera : public AbstractPeriphery {
public:
    DayCamera(std::shared_ptr<HwInterface> communication_interface,
                               std::shared_ptr<ProtocolInterface> protocol_interface) :
                               AbstractPeriphery(std::move(communication_interface), std::move(protocol_interface)) {};
    ~DayCamera() override = default;
    uint8_t init() override;
    uint8_t deinit() override;
    uint8_t getStatus() override;
    uint8_t getZoom();
    uint8_t zoomIn();
    uint8_t zoomOut();
};

#endif //PERIPHERY_MANAGER_DAYCAMERA_H
