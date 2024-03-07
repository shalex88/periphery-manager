#ifndef PERIPHERY_MANAGER_TCPSERVER_H
#define PERIPHERY_MANAGER_TCPSERVER_H

#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <netinet/in.h>

class TcpServer {
public:
    std::atomic<bool> terminate_server_{false};
    std::thread server_thread_;
    int server_socket_;
    int port_;

    TcpServer(int port) : server_socket_(-1), port_(port) {}

    bool init() {
        terminate_server_ = false;
        server_thread_ = std::thread(&TcpServer::runServer, this);

        return true;
    }

    bool deinit() {
        terminate_server_ = true;
        if (server_socket_ != -1) {
            close(server_socket_);
            server_socket_ = -1;
        }
        if (server_thread_.joinable()) {
            server_thread_.join();
        }

        std::cout << "[TCP Server] Stopped" << std::endl;

        return true;
    }

    ~TcpServer() {
        deinit();
    }

private:
    void runServer() {
        sockaddr_in server_addr{};
        int opt = 1;

        // Creating socket file descriptor
        if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port
        if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port_);

        if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
            perror("bind");
            exit(EXIT_FAILURE);
        }
        listen(server_socket_, 5);

        std::cout << "[TCP Server] Started" << std::endl;

        while (!terminate_server_) {
            sockaddr_in client_addr{};
            socklen_t client_addr_len = sizeof(client_addr);
            int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
            if (client_socket < 0) {
                if (terminate_server_) break; // Accept can fail if server is stopped
                continue;
            }
            std::cout << "[TCP Server] Client connected" << std::endl;

            handleClient(client_socket);
        }
    }

    bool handleMessage(int socket, uint8_t *buffer, size_t length) {
        // Echo received data back to client
        std::cout << "[TCP Server] Received: ";
        for (int i{}; i < length; i++) {
            std::cout << static_cast<int>(buffer[i]) << " ";
        }
        std::cout << std::endl;
        this->write(socket, buffer, length);

        return false;
    }

    void handleClient(int client_socket) {
        while (!terminate_server_) {
            uint8_t buffer[1024] = {0};
            ssize_t bytes_read = this->read(client_socket, buffer, sizeof(buffer));
            if (bytes_read <= 0) {
                break;
            }
            handleMessage(client_socket, buffer, bytes_read);
        }
        std::cout << "[TCP Server] Client disconnected" << std::endl;
        close(client_socket);
    }

    ssize_t read(int socket, uint8_t* buffer, size_t length) {
        return ::read(socket, buffer, length);
    }

    ssize_t write(int socket, const uint8_t* buffer, size_t length) {
        return ::send(socket, buffer, length, 0);
    }
};

#endif //PERIPHERY_MANAGER_TCPSERVER_H
