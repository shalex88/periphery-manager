#ifndef PERIPHERY_MANAGER_INPUTINTERFACE_H
#define PERIPHERY_MANAGER_INPUTINTERFACE_H

#include <string>
#include <system_error>
#include <vector>
#include <memory>

class InputInterface {
public:
    virtual ~InputInterface() = default;
    virtual std::error_code init() = 0;
    virtual int acceptConnection() = 0;
    virtual std::pair<std::vector<char>, bool> readData(int client) = 0;
    [[nodiscard]] virtual std::error_code sendData(int client, const std::vector<char>& data) = 0;
    virtual std::error_code closeConnection() = 0;
};

#endif //PERIPHERY_MANAGER_INPUTINTERFACE_H
