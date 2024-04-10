#ifndef PERIPHERY_MANAGER_TCPMESSAGESERVER_H
#define PERIPHERY_MANAGER_TCPMESSAGESERVER_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <list>
#include "TasksManager/CommandDispatcher.h"
#include "InputInterface.h"

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
    void stopAllClientThreads();
    std::shared_ptr<CommandDispatcher> command_dispatcher_;
    std::atomic<bool> terminate_server_{false};
    std::list<std::thread> client_threads_;
    std::mutex client_threads_mutex_;
    std::thread server_thread_;
    int server_socket_ {-1};
    int port_;
};

#endif //PERIPHERY_MANAGER_TCPMESSAGESERVER_H
