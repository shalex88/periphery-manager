#include "DayCamera.h"
#include <iostream>
#include <unordered_map>

enum class DAYCAM_COMMAND {
    GET_STATUS,
    GET_ZOOM,
    SET_ZOOM
};

std::unordered_map<DAYCAM_COMMAND, std::vector<uint8_t>> daycam_commands = {
    {DAYCAM_COMMAND::GET_STATUS, {1}},
    {DAYCAM_COMMAND::GET_ZOOM, {2}},
    {DAYCAM_COMMAND::SET_ZOOM, {3, 0}}
};

constexpr uint8_t kMinZoom = 1;
constexpr uint8_t kMaxZoom = 10;

uint8_t DayCamera::init() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    //TODO: implement

    return 0;
}

uint8_t DayCamera::deinit() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    //TODO: implement

    return 0;
}

uint8_t DayCamera::getStatus() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto status = getDataSyncronously(daycam_commands[DAYCAM_COMMAND::GET_STATUS]).at(1);

    return status;
}

uint8_t DayCamera::getZoom() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto current_zoom = getDataSyncronously(daycam_commands[DAYCAM_COMMAND::GET_ZOOM]).at(1);

    return current_zoom;
}

uint8_t DayCamera::zoomIn() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto current_zoom = getZoom();
    auto new_zoom = 0;

    if(current_zoom < kMaxZoom) {
        daycam_commands[DAYCAM_COMMAND::SET_ZOOM].back() = ++current_zoom;
        new_zoom = getDataSyncronously(daycam_commands[DAYCAM_COMMAND::SET_ZOOM]).at(1);
    } else {
        throw std::runtime_error("Maximum zoom reached");
    }

    return new_zoom;
}

uint8_t DayCamera::zoomOut() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    auto current_zoom = getZoom();
    auto new_zoom = 0;

    if(current_zoom > kMinZoom) {
        daycam_commands[DAYCAM_COMMAND::SET_ZOOM].back() = --current_zoom;
        new_zoom = getDataSyncronously(daycam_commands[DAYCAM_COMMAND::SET_ZOOM]).at(1);
    } else {
        throw std::runtime_error("Minimal zoom reached");
    }

    return new_zoom;
}

