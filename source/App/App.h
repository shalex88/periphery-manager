#ifndef PERIPHERY_MANAGER_APP_H
#define PERIPHERY_MANAGER_APP_H

class App {
public:
    App() = default;
    ~App() = default;
    static void run();
private:
    static void shutdown();
    static void setupSignalHandling();
    static bool keep_running_;
};

#endif //PERIPHERY_MANAGER_APP_H