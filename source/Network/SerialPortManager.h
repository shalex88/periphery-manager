#ifndef PERIPHERY_MANAGER_SERIALPORTMANAGER_H
#define PERIPHERY_MANAGER_SERIALPORTMANAGER_H

#include <vector>
#include "InputInterface.h"

class SerialPortManager : public InputInterface {
public:
    explicit SerialPortManager() = default;
    ~SerialPortManager() override = default;
    std::error_code init() override;
    int acceptConnection() override;
    std::pair<std::vector<char>, bool> readData(int client) override;
    std::error_code sendData(int client, const std::vector<char>& data) override;
    void closeConnection() override;

private:
    int client_id_{-1};
    void stripLineTerminators(std::vector<char>& buffer);
};

#endif //PERIPHERY_MANAGER_SERIALPORTMANAGER_H
