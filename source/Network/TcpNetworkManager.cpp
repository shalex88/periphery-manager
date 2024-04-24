#include "Network/TcpNetworkManager.h"
#include "Logger/Logger.h"
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <thread>

TcpNetworkManager::TcpNetworkManager(int port) :
        port_(port) {}

std::error_code TcpNetworkManager::init() {
    sockaddr_in server_addr{};
    int opt = 1;

    // Creating socket file descriptor
    if (server_socket_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); server_socket_ <= 0) {
        return {errno, std::generic_category()};
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
        close(server_socket_);
        return {errno, std::generic_category()};
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) != 0) {
        close(server_socket_);
        return {errno, std::generic_category()};
    }

    listen(server_socket_, max_clients_num_);

    return {};
}

int TcpNetworkManager::acceptConnection() {
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);

    int client_socket = accept4(server_socket_, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len, SOCK_CLOEXEC | O_NONBLOCK);
    if (client_socket < 0) {
        // Non-blocking mode or an actual error
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // No connections are pending, so do something else or just yield the CPU
            std::this_thread::yield(); // This is one way to avoid busy waiting
        } else {
            return -1;
        }
    } else {
        return client_socket;
    }

    return -1;
}

std::pair<char*, ssize_t> TcpNetworkManager::readData(int client_socket) {
    fd_set read_fds;
    struct timeval tv {};
    ssize_t bytes_read {-1};
    static char buffer[1024] {}; // Adjust buffer size as needed
//    std::array<char, 1024> buffer {};

    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);

    // Set timeout to 1 second. Adjust as needed for responsiveness vs. resource usage
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (select(client_socket + 1, &read_fds, nullptr, nullptr, &tv) >= 0 ) {
        memset(buffer, 0, sizeof(buffer));
        bytes_read = ::read(client_socket, buffer, sizeof(buffer) - 1);
//        bytes_read = ::read(client_socket, buffer.data(), sizeof(buffer.max_size()) - 1);
//        std::fill(buffer.begin(), buffer.end(), 0);
        if (bytes_read > 0) {
//            return {buffer.data(), bytes_read};
            return {buffer, bytes_read};
        } else if (bytes_read == 0) {
            bytes_read = -1;
        } else {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                LOG_ERROR("[Message Server] Reading failed");
            }
            bytes_read = 0;
        }
    } else {
        LOG_ERROR("[Message Server] Listening to socket failed");
    }

//    return {buffer.data(), bytes_read};
    return {buffer, bytes_read};
}

std::error_code TcpNetworkManager::sendData(int client_socket, const void* data, size_t data_size) {
    if (send(client_socket, data, data_size, 0) < 0) {
        return {errno, std::generic_category()};
    }

    return {};
}

void TcpNetworkManager::closeConnection() {
    if (server_socket_ != -1) {
        close(server_socket_);
        server_socket_ = -1;
    }
}

int TcpNetworkManager::getServerSocket() {
    return server_socket_;
}
