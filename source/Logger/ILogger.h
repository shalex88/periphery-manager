#ifndef PERIPHERY_MANAGER_ILOGGER_H
#define PERIPHERY_MANAGER_ILOGGER_H

#include <string>
#include <fmt/format.h>

class ILogger {
public:
    virtual ~ILogger() = default;

    template<typename... Args>
    void trace(const std::string& format, Args&&... args) {
        log("trace", fmt::format(format, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void debug(const std::string& format, Args&&... args) {
        log("debug", fmt::format(format, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void info(const std::string& format, Args&&... args) {
        log("info", fmt::format(format, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void warn(const std::string& format, Args&&... args) {
        log("warn", fmt::format(format, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void error(const std::string& format, Args&&... args) {
        log("error", fmt::format(format, std::forward<Args>(args)...));
    }

    template<typename... Args>
    void critical(const std::string& format, Args&&... args) {
        log("critical", fmt::format(format, std::forward<Args>(args)...));
    }

protected:
    virtual void log(const std::string& level, const std::string& message) = 0;
};

#endif //PERIPHERY_MANAGER_ILOGGER_H