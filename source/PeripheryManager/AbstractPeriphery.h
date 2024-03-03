#ifndef PERIPHERY_MANAGER_ABSTRACTPERIPHERY_H
#define PERIPHERY_MANAGER_ABSTRACTPERIPHERY_H

#include <memory>
#include <utility>
#include <future>
#include "PeripheryManager/CommunicationInterface.h"
#include "PeripheryManager/ProtocolInterface.h"

class AbstractPeriphery {
protected:
    explicit AbstractPeriphery(std::shared_ptr<CommunicationInterface> communication_interface,
                               std::shared_ptr<ProtocolInterface> protocol_interface) :
                               communication_interface_(std::move(communication_interface)),
                               protocol_interface_(std::move(protocol_interface)) {};
    virtual ~AbstractPeriphery() = default;
    std::vector<uint8_t> getDataSyncronously(const std::vector<uint8_t> &tx_data);
    std::future<std::vector<uint8_t>> getDataAsynchronously(const std::vector<uint8_t> &tx_data);
public:
    bool init();
    bool deinit();
    virtual uint8_t getStatus() = 0;
private:
    std::shared_ptr<CommunicationInterface> communication_interface_;
    std::shared_ptr<ProtocolInterface> protocol_interface_;
    std::vector<uint8_t> readData();
    bool writeData(const std::vector<uint8_t> &tx_data);
    virtual bool enable() = 0;
    virtual bool disable() = 0;
};

#endif //PERIPHERY_MANAGER_ABSTRACTPERIPHERY_H
