#ifndef PERIPHERY_MANAGER_LOGGER_H
#define PERIPHERY_MANAGER_LOGGER_H

#include "Logger/ILogger.h"
#include "Logger/SpdLogAdapter.h"
#include <memory>

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void setLogLevel(ILogger::LogLevel level) {
        logger_->setLogLevel(level);
    }

    template<typename... Args>
    void log(ILogger::LogLevel level, const std::string& format, Args... args) {
        logger_->log(level, format, std::forward<Args>(args)...);
    }

private:
    Logger() {
        logger_ = std::make_shared<SpdLogAdapter>();
    }

    std::shared_ptr<ILogger> logger_;
};

#define SET_LOG_LEVEL(level) Logger::getInstance().setLogLevel(level)

#define LOG_TRACE(...) Logger::getInstance().log(ILogger::LogLevel::Trace, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::getInstance().log(ILogger::LogLevel::Debug, __VA_ARGS__)
#define LOG_INFO(...) Logger::getInstance().log(ILogger::LogLevel::Info, __VA_ARGS__)
#define LOG_WARN(...) Logger::getInstance().log(ILogger::LogLevel::Warn, __VA_ARGS__)
#define LOG_ERROR(...) Logger::getInstance().log(ILogger::LogLevel::Error, __VA_ARGS__)
#define LOG_CRITICAL(...) Logger::getInstance().log(ILogger::LogLevel::Critical, __VA_ARGS__)

#endif //PERIPHERY_MANAGER_LOGGER_H
