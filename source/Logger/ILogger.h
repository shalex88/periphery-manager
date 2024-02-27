#ifndef PERIPHERY_MANAGER_ILOGGER_H
#define PERIPHERY_MANAGER_ILOGGER_H

#include <string>
#include <fmt/format.h>

class ILogger {
public:
    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    virtual ~ILogger() = default;

    // Template method for logging with formatting, using LogLevel enum
    template<typename... Args>
    void log(LogLevel level, const std::string &format, Args &&... args) {
        logImpl(level, fmt::format(format, std::forward<Args>(args)...));
    }

    virtual void setLogLevel(LogLevel level) = 0;

protected:
    // Pure virtual method to be implemented by derived classes, accepting LogLevel enum
    virtual void logImpl(LogLevel level, const std::string &msg) = 0;
};

#endif //PERIPHERY_MANAGER_ILOGGER_H