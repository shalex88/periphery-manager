#ifndef PERIPHERY_MANAGER_STDOUTADAPTER_H
#define PERIPHERY_MANAGER_STDOUTADAPTER_H

#include <iostream>
#include "Logger/LoggerInterface.h"

class StdoutAdapter : public LoggerInterface {
public:
    StdoutAdapter() = default;

    void setLogLevel(LogLevel level) override {
        if (level <= LoggerInterface::LogLevel::kCritical) {
            log_level_ = level;
        } else {
            throw std::invalid_argument("Invalid log severity");
        }
    }

protected:
    void logImpl(LogLevel level, const std::string &msg) override {
        if (level >= log_level_) {
            std::cout << "[" << toSpdLogLevel(level) << "] " << msg << std::endl;
        }
    }

private:
    LogLevel log_level_ = LoggerInterface::LogLevel::kInfo;

    std::string toSpdLogLevel(LogLevel level) {
        switch (level) {
            case LogLevel::kTrace:
                return "trace";
            case LogLevel::kDebug:
                return "debug";
            case LogLevel::kInfo:
                return "info";
            case LogLevel::kWarn:
                return "warning";
            case LogLevel::kError:
                return "error";
            case LogLevel::kCritical:
                return "critical";
            default:
                throw std::invalid_argument("Invalid log severity");
        }
    }
};

#endif //PERIPHERY_MANAGER_STDOUTADAPTER_H