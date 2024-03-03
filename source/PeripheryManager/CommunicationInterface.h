#ifndef PERIPHERY_MANAGER_COMMUNICATIONINTERFACE_H
#define PERIPHERY_MANAGER_COMMUNICATIONINTERFACE_H

#include <vector>
#include <cstdint>

class CommunicationInterface {
public:
    virtual ~CommunicationInterface() = default;
    virtual std::vector<uint8_t> read() = 0;
    virtual uint8_t write(const std::vector<uint8_t>& tx_data) = 0;
    virtual bool init() = 0;
    virtual bool deinit() = 0;
};

#endif //PERIPHERY_MANAGER_COMMUNICATIONINTERFACE_H
