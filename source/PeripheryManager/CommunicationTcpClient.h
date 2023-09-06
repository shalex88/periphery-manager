#ifndef PERIPHERY_MANAGER_COMMUNICATIONTCPCLIENT_H
#define PERIPHERY_MANAGER_COMMUNICATIONTCPCLIENT_H

#include <vector>
#include <arpa/inet.h>
#include "CommunicationInterface.h"

class CommunicationTcpClient : public CommunicationInterface {
public:
    CommunicationTcpClient(const std::string& server_ip, int server_port);
    ~CommunicationTcpClient() override;
    std::vector<uint8_t> read() override;
    uint8_t write(const std::vector<uint8_t>& tx_data) override;
    bool init() override;

private:
    int client_socket_;
    sockaddr_in server_address_;
};

#endif //PERIPHERY_MANAGER_COMMUNICATIONTCPCLIENT_H
