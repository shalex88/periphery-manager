#ifndef PERIPHERY_MANAGER_SPDLOGADAPTER_H
#define PERIPHERY_MANAGER_SPDLOGADAPTER_H

#include "Logger/ILogger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class SpdLogAdapter : public ILogger {
public:
    static SpdLogAdapter &getInstance() {
        static SpdLogAdapter instance;
        return instance;
    }

    SpdLogAdapter(const SpdLogAdapter &) = delete;

    SpdLogAdapter &operator=(const SpdLogAdapter &) = delete;

    ~SpdLogAdapter() override {
        spdlog::drop_all();
    }

    void setLogLevel(const std::string& level) override {
        auto logger = spdlog::get("console");

        if (level == "trace") log_level_ = spdlog::level::trace;
        else if (level == "debug") log_level_ = spdlog::level::debug;
        else if (level == "info") log_level_ = spdlog::level::info;
        else if (level == "warn") log_level_ = spdlog::level::warn;
        else if (level == "error") log_level_ = spdlog::level::err;
        else if (level == "critical") log_level_ = spdlog::level::critical;

        logger->set_level(log_level_);
    }

    spdlog::level::level_enum getLogLevel() {
        return log_level_;
    }

protected:
    void log(const std::string &level, const std::string &message) override {
        auto logger = spdlog::get("console");

        if (level == "trace") logger->trace(message);
        else if (level == "debug") logger->debug(message);
        else if (level == "info") logger->info(message);
        else if (level == "warn") logger->warn(message);
        else if (level == "error") logger->error(message);
        else if (level == "critical") logger->critical(message);
    }

private:
    SpdLogAdapter() {
        auto logger = spdlog::stdout_color_mt("console");
        spdlog::set_level(log_level_);
    }

    spdlog::level::level_enum log_level_ = spdlog::level::info;
};

#endif //PERIPHERY_MANAGER_SPDLOGADAPTER_H