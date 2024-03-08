#ifndef PERIPHERY_MANAGER_TCPSERVER_H
#define PERIPHERY_MANAGER_TCPSERVER_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

class TcpServer {
public:
    std::atomic<bool> terminate_server_{false};
    std::thread server_thread_;
    int server_socket_;
    int port_;

    explicit TcpServer(int port);
    ~TcpServer();
    bool init();
    bool deinit();

private:
    void runServer();
    bool handleMessage(int socket, char* buffer, size_t length);
    void handleClient(int client_socket);
    ssize_t read(int socket, char* buffer, size_t length);
    ssize_t write(int socket, const char* buffer, size_t length);
};

#endif //PERIPHERY_MANAGER_TCPSERVER_H
