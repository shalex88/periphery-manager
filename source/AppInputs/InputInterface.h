#ifndef PERIPHERY_MANAGER_INPUTINTERFACE_H
#define PERIPHERY_MANAGER_INPUTINTERFACE_H

#include <string>
#include <utility>

class InputInterface : public std::enable_shared_from_this<InputInterface> {
public:
    struct Requester {
        std::shared_ptr<InputInterface> input_interface_;
        int id_;
        Requester(std::shared_ptr<InputInterface> input_interface, int id) : input_interface_(std::move(input_interface)), id_(id) {}
    };
    virtual void sendResponse(std::shared_ptr<InputInterface::Requester> requester, const std::string& response) = 0;
    virtual ~InputInterface() = default;
};

#endif //PERIPHERY_MANAGER_INPUTINTERFACE_H
