#ifndef PERIPHERY_MANAGER_APPMANAGER_H
#define PERIPHERY_MANAGER_APPMANAGER_H

class AppManager {
public:
    AppManager() = default;
    ~AppManager() = default;
    static int run();
private:
    bool init();
    static void shutdown();
    static void setupSignalHandling();
    static bool keep_running_;
};

#endif //PERIPHERY_MANAGER_APPMANAGER_H