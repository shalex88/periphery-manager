#ifndef PERIPHERY_MANAGER_LOGGER_H
#define PERIPHERY_MANAGER_LOGGER_H

#include "ILogger.h"
#include "SpdLogAdapter.h"

inline ILogger& getGlobalLogger() {
    return SpdLogAdapter::getInstance();
}

#define LOG_TRACE(fmt, ...) getGlobalLogger().trace(fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) getGlobalLogger().debug(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  getGlobalLogger().info(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  getGlobalLogger().warn(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) getGlobalLogger().error(fmt, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) getGlobalLogger().critical(fmt, ##__VA_ARGS__)

#endif //PERIPHERY_MANAGER_LOGGER_H