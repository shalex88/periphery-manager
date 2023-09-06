#include <vector>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "CommunicationTcpClient.h"

CommunicationTcpClient::CommunicationTcpClient(const std::string &server_ip, int server_port)
        : client_socket_(-1), server_address_{} {
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    server_address_.sin_family = AF_INET;
    server_address_.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip.c_str(), &server_address_.sin_addr);
}

CommunicationTcpClient::~CommunicationTcpClient() {
    if (client_socket_ != -1) {
        close(client_socket_);
    }
}

std::vector<uint8_t> CommunicationTcpClient::read() {
    uint8_t buffer[256];
    ssize_t bytes_received = recv(client_socket_, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        return {};
    }
    return {buffer, buffer + bytes_received};
}

uint8_t CommunicationTcpClient::write(const std::vector<uint8_t> &tx_data) {
    if (send(client_socket_, tx_data.data(), tx_data.size(), 0) != -1) {
        return tx_data.size();
    }

    return 0;
}

bool CommunicationTcpClient::init() {
    return connect(client_socket_, (struct sockaddr*) &server_address_, sizeof(server_address_)) != -1;
}