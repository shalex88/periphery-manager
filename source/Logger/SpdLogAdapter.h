#ifndef PERIPHERY_MANAGER_SPDLOGADAPTER_H
#define PERIPHERY_MANAGER_SPDLOGADAPTER_H

#include "ILogger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class SpdLogAdapter : public ILogger {
public:
    static SpdLogAdapter& getInstance() {
        static SpdLogAdapter instance;
        return instance;
    }

    SpdLogAdapter(const SpdLogAdapter&) = delete;
    SpdLogAdapter& operator=(const SpdLogAdapter&) = delete;

protected:
    void log(const std::string& level, const std::string& message) override {
        auto logger = spdlog::get("console");
        if (!logger) {
            logger = spdlog::stdout_color_mt("console");
            spdlog::set_level(spdlog::level::debug);
        }

        if(level == "trace") logger->trace(message);
        else if(level == "debug") logger->debug(message);
        else if(level == "info") logger->info(message);
        else if(level == "warn") logger->warn(message);
        else if(level == "error") logger->error(message);
        else if(level == "critical") logger->critical(message);
    }

private:
    SpdLogAdapter() {}
};

#endif //PERIPHERY_MANAGER_SPDLOGADAPTER_H