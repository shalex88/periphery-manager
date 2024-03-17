#ifndef PERIPHERY_MANAGER_TCPMESSAGESERVER_H
#define PERIPHERY_MANAGER_TCPMESSAGESERVER_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include "TasksManager/Scheduler.h"

class TcpMessageServer {
public:
    std::atomic<bool> terminate_server_{false};
    std::thread server_thread_;
    int server_socket_ = -1;
    int port_;

    explicit TcpMessageServer(int port); //TODO: remove
    TcpMessageServer(int port, std::shared_ptr<Scheduler>);
    ~TcpMessageServer();
    bool init();
    bool deinit();

private:
    void runServer();
    bool handleMessage(int socket, char* buffer, size_t length);
    void handleClient(int client_socket);
    ssize_t read(int socket, char* buffer, size_t length);
    ssize_t write(int socket, const char* buffer, size_t length);
    bool handleCommand(const std::string& command);
    std::shared_ptr<Scheduler> scheduler_;
};

#endif //PERIPHERY_MANAGER_TCPMESSAGESERVER_H
