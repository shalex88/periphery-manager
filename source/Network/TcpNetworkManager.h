#ifndef PERIPHERY_MANAGER_TCPNETWORKMANAGER_H
#define PERIPHERY_MANAGER_TCPNETWORKMANAGER_H

#include "NetworkInterface.h"

class TcpNetworkManager : public NetworkInterface {
public:
    explicit TcpNetworkManager(int port);
    ~TcpNetworkManager() override = default;
    std::error_code init() override;
    int acceptConnection() override;
    std::pair<char*, ssize_t> readData(int client_socket) override;
    std::error_code sendData(int client_socket, const void* data, size_t data_size) override;
    void closeConnection() override;
    int getServerSocket() override;

private:
    int server_socket_ {-1};
    int port_ {-1};
    int max_clients_num_ {5};
};

#endif //PERIPHERY_MANAGER_TCPNETWORKMANAGER_H
