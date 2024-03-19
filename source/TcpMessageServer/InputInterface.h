#ifndef PERIPHERY_MANAGER_INPUTINTERFACE_H
#define PERIPHERY_MANAGER_INPUTINTERFACE_H

#include <string>

class InputInterface : public std::enable_shared_from_this<InputInterface> {
public:
    virtual bool sendResponse(const std::string& response) = 0;
    virtual ~InputInterface() = default;
};

#endif //PERIPHERY_MANAGER_INPUTINTERFACE_H
