#ifndef PERIPHERY_MANAGER_TCPCLIENT_H
#define PERIPHERY_MANAGER_TCPCLIENT_H

#include <string>
#include <arpa/inet.h>
#include "PeripheryManager/HwInterface.h"

class TcpClient : public HwInterface {
public:
    TcpClient(const std::string& server_ip, int server_port);
    ~TcpClient() override = default;
    std::vector<uint8_t> read() override;
    uint8_t write(const std::vector<uint8_t>& tx_data) override;
    bool init() override;
    bool deinit() override;

private:
    int client_socket_;
    sockaddr_in server_address_;
};

#endif //PERIPHERY_MANAGER_TCPCLIENT_H
