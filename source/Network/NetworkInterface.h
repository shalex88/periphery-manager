#ifndef PERIPHERY_MANAGER_NETWORKINTERFACE_H
#define PERIPHERY_MANAGER_NETWORKINTERFACE_H

#include <string>
#include <system_error>

class NetworkInterface {
public:
    virtual ~NetworkInterface() = default;
    virtual std::error_code init() = 0;
    virtual int acceptConnection() = 0;
    virtual std::pair<char*, ssize_t> readData(int client_socket) = 0;
    virtual std::error_code sendData(int client_socket, const void* data, size_t data_size) = 0;
    virtual void closeConnection() = 0;
    virtual int getServerSocket() = 0;
};

#endif //PERIPHERY_MANAGER_NETWORKINTERFACE_H
