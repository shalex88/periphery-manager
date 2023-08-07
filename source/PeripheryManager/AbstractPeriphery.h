#ifndef PERIPHERY_MANAGER_ABSTRACTPERIPHERY_H
#define PERIPHERY_MANAGER_ABSTRACTPERIPHERY_H

#include <memory>
#include <utility>
#include "CommunicationInterface.h"

class AbstractPeriphery {
public:
    explicit AbstractPeriphery(std::shared_ptr<CommunicationInterface> interface) : interface_(std::move(interface)) {};
    virtual ~AbstractPeriphery() = default;
    virtual uint8_t init() = 0;
    virtual uint8_t deinit() = 0;
    virtual uint8_t getStatus() = 0;
    std::vector<uint8_t> readData();
    uint8_t writeData(std::vector<uint8_t>& tx_data);
private:
    std::shared_ptr<CommunicationInterface> interface_;
};


#endif //PERIPHERY_MANAGER_ABSTRACTPERIPHERY_H
