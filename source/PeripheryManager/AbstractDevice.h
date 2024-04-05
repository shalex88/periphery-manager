#ifndef PERIPHERY_MANAGER_ABSTRACTDEVICE_H
#define PERIPHERY_MANAGER_ABSTRACTDEVICE_H

#include <memory>
#include <utility>
#include <future>
#include <optional>
#include "PeripheryManager/HwInterface.h"
#include "PeripheryManager/ProtocolInterface.h"

class AbstractDevice {
protected:
    explicit AbstractDevice(std::shared_ptr<HwInterface> communication_interface,
                            std::shared_ptr<ProtocolInterface> protocol_interface) :
                               communication_interface_(std::move(communication_interface)),
                               protocol_interface_(std::move(protocol_interface)) {};
    virtual ~AbstractDevice() = default;
    std::optional<std::vector<uint8_t>> getDataSyncronously(const std::vector<uint8_t> &tx_data);
    std::future<std::vector<uint8_t>> getDataAsynchronously(const std::vector<uint8_t> &tx_data);
public:
    bool init();
    bool deinit();
private:
    std::shared_ptr<HwInterface> communication_interface_;
    std::shared_ptr<ProtocolInterface> protocol_interface_;
    bool is_enabled_{false};
    std::vector<uint8_t> readData();
    bool writeData(const std::vector<uint8_t> &tx_data);
    virtual bool enable() = 0;
    virtual bool disable() = 0;
    bool isEnabled() const;
};

#endif //PERIPHERY_MANAGER_ABSTRACTDEVICE_H
