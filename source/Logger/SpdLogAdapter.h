#ifndef PERIPHERY_MANAGER_SPDLOGADAPTER_H
#define PERIPHERY_MANAGER_SPDLOGADAPTER_H

#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/ostream_sink.h"
#include "Logger/LoggerInterface.h"

class SpdLogAdapter : public LoggerInterface {
public:
    SpdLogAdapter() {
        auto stdout_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout, true);
        logger_ = std::make_shared<spdlog::logger>("console", stdout_sink);
        spdlog::set_default_logger(logger_);
        logger_->set_level(spdlog::level::info);
    }

    ~SpdLogAdapter() override {
        spdlog::drop_all();
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

    spdlog::level::level_enum toSpdLogLevel(LogLevel level) {
        switch (level) {
            case LogLevel::kTrace:
                return spdlog::level::trace;
            case LogLevel::kDebug:
                return spdlog::level::debug;
            case LogLevel::kInfo:
                return spdlog::level::info;
            case LogLevel::kWarn:
                return spdlog::level::warn;
            case LogLevel::kError:
                return spdlog::level::err;
            case LogLevel::kCritical:
                return spdlog::level::critical;
            default:
                return spdlog::level::info;
        }
    }
};

#endif //PERIPHERY_MANAGER_SPDLOGADAPTER_H