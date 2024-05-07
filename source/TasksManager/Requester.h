#ifndef PERIPHERY_MANAGER_REQUESTER_H
#define PERIPHERY_MANAGER_REQUESTER_H

#include "Network/InputInterface.h"

class Requester {
public:
    std::shared_ptr<InputInterface> source;
    int source_id;
    Requester(std::shared_ptr<InputInterface> input_interface, int id) : source(std::move(input_interface)), source_id(id) {}
};

#endif //PERIPHERY_MANAGER_REQUESTER_H
