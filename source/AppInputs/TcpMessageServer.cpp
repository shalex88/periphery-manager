#include <string>
#include <sstream>
#include <utility>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "Logger/Logger.h"
#include "TcpMessageServer.h"

TcpMessageServer::TcpMessageServer(int port, std::shared_ptr<CommandDispatcher> command_dispatcher) :
        port_(port), command_dispatcher_(std::move(command_dispatcher)) {
}

TcpMessageServer::~TcpMessageServer() {
    deinit();
}

bool TcpMessageServer::init() {
    terminate_server_ = false;
    server_thread_ = std::thread(&TcpMessageServer::runServer, this);

    return true;
}

bool TcpMessageServer::deinit() {
    terminate_server_ = true;

    stopAllClientThreads();

    if (server_socket_ != -1) {
        close(server_socket_);
        server_socket_ = -1;
    }
    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    LOG_INFO("[TCP Server] Stopped");

    return true;
}

void TcpMessageServer::runServer() {
    sockaddr_in server_addr{};
    int opt = 1;

    // Creating socket file descriptor
    if ((server_socket_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == 0) {
        LOG_ERROR("[TCP Server] Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        LOG_ERROR("[TCP Server] Socket attach failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr*) &server_addr, sizeof(server_addr))) {
        LOG_ERROR("[TCP Server] Binding failed");
        exit(EXIT_FAILURE);
    }
    listen(server_socket_, 5);

    LOG_INFO("[TCP Server] Started");

    while (!terminate_server_) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            // Non-blocking mode or an actual error
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                // No connections are pending, so do something else or just yield the CPU
                std::this_thread::yield(); // This is one way to avoid busy waiting
            } else {
                // Handle other errors that might have occurred
                LOG_ERROR("[TCP Server] Accept failed with error");
            }
        } else {
            LOG_TRACE("[TCP Server] Client {} connected", client_socket);

            std::lock_guard<std::mutex> lock(client_threads_mutex_);
            client_threads_.emplace_back(&TcpMessageServer::handleClient, this, client_socket);
        }
    }
}

void TcpMessageServer::handleClient(int client_socket) {
    // Set client socket to non-blocking mode
    int flags = fcntl(client_socket, F_GETFL, 0);
    if (flags == -1) {
        LOG_ERROR("[TCP Server] Error getting socket flags");
        return;
    }
    if (fcntl(client_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        LOG_ERROR("[TCP Server] Error setting socket to non-blocking");
        return;
    }

    fd_set read_fds;
    struct timeval tv{};
    int retval;
    ssize_t bytes_read;
    char buffer[1024]; // Adjust buffer size as needed

    while (!terminate_server_) {
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);

        // Set timeout to 1 second. Adjust as needed for responsiveness vs. resource usage
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        retval = select(client_socket + 1, &read_fds, nullptr, nullptr, &tv);

        if (retval == -1) {
            LOG_ERROR("[TCP Server] Listening to socket failed");
            break;
        } else if (retval) {
            // Data is available to be read
            memset(buffer, 0, sizeof(buffer));
            bytes_read = ::read(client_socket, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                getRequest(client_socket, buffer, bytes_read);
            } else if (bytes_read == 0) {
                LOG_TRACE("[TCP Server] Client {} disconnected", client_socket);
                break;
            } else {
                if (errno != EWOULDBLOCK && errno != EAGAIN) {
                    LOG_ERROR("[TCP Server] Reading failed");
                    break;
                }
            }
        }
    }

    close(client_socket);
    LOG_TRACE("[TCP Server] Client {} disconnected", client_socket);
}

bool TcpMessageServer::getRequest(int client, char* buffer, size_t length) {
    // Initial part of the message
    std::ostringstream os;
    os << "[TCP Server] Received from client " << client <<" (" << length << " bytes): ";

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

bool TcpMessageServer::sendResponse(std::shared_ptr<InputInterface::Requester> requester, const std::string& response) {
    auto bytes_sent = send(requester->id_, response.c_str(), response.size(), 0);

    if (bytes_sent > 0) {
        return true;
    }

    return false;
}

void TcpMessageServer::stopAllClientThreads() {
    if (!client_threads_.empty()) {
        for (auto& thread: client_threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        client_threads_.clear();
    }
}
