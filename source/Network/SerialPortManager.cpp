#include "SerialPortManager.h"
#include "Logger/Logger.h"
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

const int MaxBufferSize{1024};

std::error_code SerialPortManager::init() {
    client_id_ = posix_openpt(O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (client_id_ < 0) {
        return {errno, std::generic_category()};
    }

    if ((grantpt(client_id_) != 0) || (unlockpt(client_id_) != 0)) {
        return {errno, std::generic_category()};
    }

    struct termios term_attr{};
    if (tcgetattr(client_id_, &term_attr) < 0) {
        return {errno, std::generic_category()};
    }

    term_attr.c_lflag &= ~(ECHO | ECHOE | ECHONL);

    if (tcsetattr(client_id_, TCSANOW, &term_attr) < 0) {
        return {errno, std::generic_category()};
    }

    LOG_INFO("Listening for requests at {}", ptsname(client_id_));

    return {};
}

std::error_code SerialPortManager::closeConnection() {
    if (client_id_ >= 0) {
        close(client_id_);
        client_id_ = -1;

        return {};
    }

    return make_error_code(std::errc::not_connected);
}

std::pair<std::vector<char>, bool> SerialPortManager::readData(int client) {
    bool terminate{true};

    if (!validClient(client)) {
        return {{}, terminate};
    }

    std::vector<char> buffer(MaxBufferSize);

    const ssize_t bytes_read = ::read(client, buffer.data(), buffer.size() - 1);
    if (bytes_read > 0) {
        stripLineTerminators(buffer);
        terminate = false;
    } else if (bytes_read == 0) {
        terminate = true;
    } else {
        buffer.clear();
        terminate = false;
    }

    return {buffer, terminate};
}

bool SerialPortManager::validClient(int client) const {
    return (client == client_id_) && (client != -1);
}

std::error_code SerialPortManager::sendData(int client, const std::vector<char>& data) {
    if (!validClient(client)) {
        return make_error_code(std::errc::not_connected);
    }

    LOG_INFO("Sending to client: {}", std::string(data.begin(), data.end()));

    if (write(client, data.data(), data.size()) < 0) {
        return {errno, std::generic_category()};
    }

    return {};
}

int SerialPortManager::acceptConnection() {
    static bool first_call = true;

    if (first_call) {
        first_call = false;
        return client_id_;
    }

    return -1;
}

void SerialPortManager::stripLineTerminators(std::vector<char>& buffer) {
    auto pos = std::find_if(buffer.begin(), buffer.end(), [](char c) {
        return c == '\r' || c == '\n' || c == '\0';
    });

    if (pos != buffer.end()) {
        buffer.resize(std::distance(buffer.begin(), pos));
    }
}