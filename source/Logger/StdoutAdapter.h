#ifndef PERIPHERY_MANAGER_STDOUTADAPTER_H
#define PERIPHERY_MANAGER_STDOUTADAPTER_H

#include <iostream>
#include "Logger/LoggerInterface.h"

class StdoutAdapter : public LoggerInterface {
public:
    StdoutAdapter() = default;

    void setLogLevel(LogLevel level) override {
        log_level_ = level;
    }

protected:
    void logImpl(LogLevel level, const std::string &msg) override {
        if (level >= log_level_) {
            std::cout << "[" << toSpdLogLevel(level) << "] " << msg << std::endl;
        }
    }

private:
    LogLevel log_level_ = LoggerInterface::LogLevel::Info;

    std::string toSpdLogLevel(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:
                return "trace";
            case LogLevel::Debug:
                return "debug";
            case LogLevel::Info:
                return "info";
            case LogLevel::Warn:
                return "warn";
            case LogLevel::Error:
                return "error";
            case LogLevel::Critical:
                return "critical";
            default:
                return "info"; // Default to info if unknown level
        }
    }
};

#endif //PERIPHERY_MANAGER_STDOUTADAPTER_H