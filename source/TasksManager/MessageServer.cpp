#include "MessageServer.h"
#include "Logger/Logger.h"
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>

MessageServer::MessageServer(std::shared_ptr<CommandDispatcher> command_dispatcher, std::vector<std::shared_ptr<InputInterface>> network_managers) :
    command_dispatcher_(std::move(command_dispatcher)), network_managers_(std::move(network_managers)) {
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
        auto ec = network_manager->closeConnection();
        if (ec) {
            LOG_ERROR("[Message Server] {}", ec.message());
            return true;
        }
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
                auto requester = std::make_shared<Requester>(network_manager, client);
                client_threads_.emplace_back(&MessageServer::handleClient, this, std::move(requester));
            }
        }
    }
}

void MessageServer::handleClient(std::shared_ptr<Requester> requester) {
    while (keep_running_) {
        auto [data, terminate] = requester->source->readData(requester->source_id);

        if (terminate) {
            break;
        }

        if (!data.empty()) {
            parseMessage(requester, data);
        }
    }

    close(requester->source_id);
}

bool MessageServer::parseMessage(std::shared_ptr<Requester> requester, const std::vector<char>& buffer) {
    LOG_TRACE("{}", printMessage(requester->source_id, buffer));

    command_dispatcher_->dispatchCommand(std::move(requester), std::string(buffer.begin(), buffer.end()));

    return true;
}

std::string MessageServer::printMessage(int client, const std::vector<char>& buffer) const {
    std::ostringstream os;
    os << "[Message Server] Received from client " << client <<" (" << buffer.size() << " bytes): ";
    os << std::string(buffer.begin(), buffer.end()) << " [";

    for (const auto& c : buffer) {
        os << static_cast<int>(static_cast<unsigned char>(c)) << " ";
    }

    os << "]";
    return os.str();
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
