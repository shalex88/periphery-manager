#ifndef PERIPHERY_MANAGER_TCPMESSAGESERVER_H
#define PERIPHERY_MANAGER_TCPMESSAGESERVER_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include "TasksManager/CommandDispatcher.h"
#include "TcpMessageServer/InputInterface.h"

class TcpMessageServer : public InputInterface {
public:
    TcpMessageServer(int port, std::shared_ptr<CommandDispatcher>);
    ~TcpMessageServer() override;
    bool init();
    bool deinit();
    bool sendResponse(std::shared_ptr<InputInterface::Requester> requester, const std::string& response) override;

private:
    void runServer();
    bool getRequest(int client, char* buffer, size_t length);
    void handleClient(int client_socket);
    ssize_t read(int socket, char* buffer, size_t length);
    ssize_t write(int socket, const char* buffer, size_t length);
    std::shared_ptr<CommandDispatcher> command_dispatcher_;
    std::atomic<bool> terminate_server_{false};
    std::thread server_thread_;
    int server_socket_ {-1};
    int port_;
    int client_socket_ {-1};
};

#endif //PERIPHERY_MANAGER_TCPMESSAGESERVER_H
