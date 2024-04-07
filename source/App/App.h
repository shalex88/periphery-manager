#ifndef PERIPHERY_MANAGER_APP_H
#define PERIPHERY_MANAGER_APP_H

#include <atomic>

class App {
public:
    App() = default;
    ~App() = default;
    static void run();
    static void shutdown();

private:
    static std::atomic<bool> keep_running_;
};

#endif //PERIPHERY_MANAGER_APP_H