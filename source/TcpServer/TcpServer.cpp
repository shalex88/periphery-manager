#include <string>
#include <sstream>
#include <netinet/in.h>
#include <unistd.h>
#include "Logger/Logger.h"
#include "TcpServer/TcpServer.h"

TcpServer::TcpServer(int port) :
    server_socket_(-1), port_(port) {}

TcpServer::~TcpServer() {
    deinit();
}

bool TcpServer::init() {
    terminate_server_ = false;
    server_thread_ = std::thread(&TcpServer::runServer, this);

    return true;
}

bool TcpServer::deinit() {
    terminate_server_ = true;
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

void TcpServer::runServer() {
    sockaddr_in server_addr{};
    int opt = 1;

    // Creating socket file descriptor
    if ((server_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(server_socket_, 5);

    LOG_INFO("[TCP Server] Started");

    while (!terminate_server_) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket_, (struct sockaddr *) &client_addr, &client_addr_len);
        if (client_socket < 0) {
            if (terminate_server_) break; // Accept can fail if server is stopped
            continue;
        }
        LOG_INFO("[TCP Server] Client connected");

        handleClient(client_socket);
    }
}

bool TcpServer::handleMessage(int socket, char* buffer, size_t length) {
    // Initial part of the message
    std::ostringstream os;
    os << "[TCP Server] Received (" << length << " bytes): \"";

    // Loop to print the character representations directly
    for (int i = 0; i < length; i++) {
        os << buffer[i];
    }

    // Closing the quoted string and starting the byte values
    os << "\" [";

    // Loop to print the byte values
    for (int i = 0; i < length; i++) {
        os << static_cast<int>(static_cast<unsigned char>(buffer[i]));
        if (i < length - 1) {
            os << " ";
        }
    }

    // Closing the byte values and the entire message
    os << "]" << std::endl;

    LOG_TRACE("{}", os.str());

    const std::string reply{"OK"};
    this->write(socket, reply.c_str(), reply.length());

    return true;
}

void TcpServer::handleClient(int client_socket) {
    while (!terminate_server_) {
        char buffer[1024] = {0};
        ssize_t bytes_read = this->read(client_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }
        handleMessage(client_socket, buffer, bytes_read);
    }
    LOG_INFO("[TCP Server] Client disconnected");
    close(client_socket);
}

ssize_t TcpServer::read(int socket, char *buffer, size_t length) {
    return ::read(socket, buffer, length);
}

ssize_t TcpServer::write(int socket, const char *buffer, size_t length) {
    return ::send(socket, buffer, length, 0);
}
