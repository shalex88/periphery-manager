#ifndef PERIPHERY_MANAGER_TCPMESSAGESERVER_H
#define PERIPHERY_MANAGER_TCPMESSAGESERVER_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include "TasksManager/CommandDispatcher.h"

class TcpMessageServer {
public:
    std::atomic<bool> terminate_server_{false};
    std::thread server_thread_;
    int server_socket_ = -1;
    int port_;

    TcpMessageServer(int port, std::shared_ptr<CommandDispatcher>);
    ~TcpMessageServer();
    bool init();
    bool deinit();

private:
    void runServer();
    bool handleMessage(char* buffer, size_t length);
    void handleClient(int client_socket);
    ssize_t read(int socket, char* buffer, size_t length);
    ssize_t write(int socket, const char* buffer, size_t length);
    std::shared_ptr<CommandDispatcher> command_dispatcher_;
};

#endif //PERIPHERY_MANAGER_TCPMESSAGESERVER_H
