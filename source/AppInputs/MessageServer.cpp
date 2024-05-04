#include "MessageServer.h"
#include "Logger/Logger.h"
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>

MessageServer::MessageServer(std::shared_ptr<CommandDispatcher> command_dispatcher, std::vector<std::shared_ptr<NetworkInterface>> network_managers) :
    command_dispatcher_(std::move(command_dispatcher)), network_managers_(std::move(network_managers)) {
}

MessageServer::MessageServer(std::shared_ptr<CommandDispatcher> command_dispatcher, std::shared_ptr<NetworkInterface> network_manager)  :
    command_dispatcher_(std::move(command_dispatcher)), network_managers_({std::move(network_manager)}) {
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

    for(const auto& network_manager : network_managers_) {
        network_manager->closeConnection();
    }

    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    LOG_INFO("[Message Server] Stopped");

    return true;
}

void MessageServer::runServer() {
    for (const auto& network_manager : network_managers_) {
        auto ec = network_manager->init();
        if (ec) {
            LOG_ERROR("[Message Server] {}", ec.message());
        }
    }

    LOG_INFO("[Message Server] Started");

    while (keep_running_) {
        for (const auto& network_manager: network_managers_) {
            if (const auto client = network_manager->acceptConnection(); client > 0) {
                std::lock_guard<std::mutex> lock(client_threads_mutex_);
                client_threads_.emplace_back(&MessageServer::handleClient, this, network_manager, client);
            }
        }
    }
}

void MessageServer::handleClient(std::shared_ptr<NetworkInterface> network_manager, int client) {
    while (keep_running_) {
        auto [data, disconnect] = network_manager->readData(client);

        if (disconnect) {
            break;
        }

        if (!data.empty()) {
            parseMessage(client, data);
        }
    }

    close(client);
}

bool MessageServer::parseMessage(const int client, const std::vector<char>& buffer) {
    LOG_TRACE("{}", printMessage(client, buffer));

    auto requester = std::make_shared<InputInterface::Requester>(shared_from_this(), client);
    command_dispatcher_->dispatchCommand(requester, std::string(buffer.begin(), buffer.end()));

    return true;
}

std::string MessageServer::printMessage(const int client, const std::vector<char>& buffer) const {
    std::ostringstream os;
    os << "[Message Server] Received from client " << client <<" (" << buffer.size() << " bytes): ";
    os << std::string(buffer.begin(), buffer.end()) << " [";

    for (const auto& c : buffer) {
        os << static_cast<int>(static_cast<unsigned char>(c)) << " ";
    }

    os << "]";
    return os.str();
}

void MessageServer::sendResponse(std::shared_ptr<InputInterface::Requester> requester, const std::string& response) {
    const std::vector<char> data(response.begin(), response.end());
    auto ec = network_managers_[0]->sendData(requester->source_id, data);
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
