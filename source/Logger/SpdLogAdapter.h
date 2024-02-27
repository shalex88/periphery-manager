#ifndef PERIPHERY_MANAGER_SPDLOGADAPTER_H
#define PERIPHERY_MANAGER_SPDLOGADAPTER_H

#include "Logger/ILogger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class SpdLogAdapter : public ILogger {
public:
    SpdLogAdapter() {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);
        logger_ = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{console_sink, file_sink});
        logger_->set_level(log_level_);
    }

    ~SpdLogAdapter() override {
        spdlog::drop_all();
        spdlog::shutdown();
    }

    void setLogLevel(LogLevel level) override {
        logger_->set_level(toSpdLogLevel(level));
    }

protected:
    void logImpl(LogLevel level, const std::string &msg) override {
        logger_->log(toSpdLogLevel(level), msg);
    }

private:
    std::shared_ptr<spdlog::logger> logger_;
    spdlog::level::level_enum log_level_ = spdlog::level::info;

    spdlog::level::level_enum toSpdLogLevel(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:
                return spdlog::level::trace;
            case LogLevel::Debug:
                return spdlog::level::debug;
            case LogLevel::Info:
                return spdlog::level::info;
            case LogLevel::Warn:
                return spdlog::level::warn;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Critical:
                return spdlog::level::critical;
            default:
                return spdlog::level::info; // Default to info if unknown level
        }
    }
};

#endif //PERIPHERY_MANAGER_SPDLOGADAPTER_H