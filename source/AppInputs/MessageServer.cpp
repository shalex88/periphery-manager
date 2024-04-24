#include "MessageServer.h"
#include "Logger/Logger.h"
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>

MessageServer::MessageServer(std::shared_ptr<CommandDispatcher> command_dispatcher, std::shared_ptr<NetworkInterface> network_manager) :
    command_dispatcher_(std::move(command_dispatcher)), network_manager_(std::move(network_manager)) {
}

MessageServer::~MessageServer() {
    deinit();
}

bool MessageServer::init() {
    keep_running_ = true;
    server_thread_ = std::thread(&MessageServer::runServer, this);

    return true;
}

bool MessageServer::deinit() {
    keep_running_ = false;

    stopAllClientThreads();

    network_manager_->closeConnection();

    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    LOG_INFO("[Message Server] Stopped");

    return true;
}

void MessageServer::runServer() {
    auto ec = network_manager_->init();
    if (ec) {
        LOG_ERROR("[Message Server] {}", ec.message());
    }

    LOG_INFO("[Message Server] Started");

    while (keep_running_) {
        if (int client_socket = network_manager_->acceptConnection(); client_socket >= 0) {
            LOG_TRACE("[Message Server] Client {} connected", client_socket);
            std::lock_guard<std::mutex> lock(client_threads_mutex_);
            client_threads_.emplace_back(&MessageServer::handleClient, this, client_socket);
        }
    }
}

void MessageServer::handleClient(int client_socket) {
    while (keep_running_) {
        auto data = network_manager_->readData(client_socket);
        if (data.second > 0) {
            parseMessage(client_socket, data.first, data.second);
        } else if (data.second < 0) {
            break;
        }
    }

    close(client_socket);
    LOG_TRACE("[Message Server] Client {} disconnected", client_socket);
}

bool MessageServer::parseMessage(int client, const char* buffer, size_t length) {
    // Initial part of the message
    std::ostringstream os;
    os << "[Message Server] Received from client " << client <<" (" << length << " bytes): ";

    os << std::string(buffer) << " [";

    // Loop to print the byte values
    for (int i = 0; i < length; i++) {
        os << static_cast<int>(static_cast<unsigned char>(buffer[i]));
        if (i < length - 1) {
            os << " ";
        }
    }

    // Closing the byte values and the entire message
    os << "]";

    LOG_TRACE("{}", os.str());

    auto requester = std::make_shared<InputInterface::Requester>(shared_from_this(), client);
    command_dispatcher_->dispatchCommand(requester, std::string(buffer));

    return true;
}

void MessageServer::sendResponse(std::shared_ptr<InputInterface::Requester> requester, const std::string& response) {
    auto ec = network_manager_->sendData(requester->id_, response.c_str(), response.size());
    if (ec) {
        LOG_ERROR("[Message Server] {}", ec.message());
    }
}

void MessageServer::stopAllClientThreads() {
    if (!client_threads_.empty()) {
        for (auto& thread: client_threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        client_threads_.clear();
    }
}
