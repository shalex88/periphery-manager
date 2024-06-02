#ifndef PERIPHERY_MANAGER_MESSAGESERVER_H
#define PERIPHERY_MANAGER_MESSAGESERVER_H

#include "TasksManager/CommandDispatcher.h"
#include "Network/InputInterface.h"
#include "TasksManager/Requester.h"
#include <atomic>
#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <mutex>

class MessageServer {
public:
    MessageServer(std::shared_ptr<CommandDispatcher> command_dispatcher, std::vector<std::shared_ptr<InputInterface>> network_managers);
    ~MessageServer();
    bool init();
    bool deinit();

private:
    void runServer();
    bool parseMessage(std::shared_ptr<Requester> requester, const std::vector<char>& buffer);
    std::string printMessage(int client, const std::vector<char>& buffer) const;
    void handleClient(std::shared_ptr<Requester> requester);
    void stopAllClientThreads();
    std::shared_ptr<CommandDispatcher> command_dispatcher_;
    std::vector<std::shared_ptr<InputInterface>> network_managers_;
    std::atomic<bool> keep_running_{false};
    std::vector<std::thread> client_threads_;
    std::mutex client_threads_mutex_;
    std::thread server_thread_;
};

#endif //PERIPHERY_MANAGER_MESSAGESERVER_H