#pragma once
#include <windows.h>
#include "TranSport.h"
#include "StreamDock.h"
#include <map>

class DeviceManager
{
public:
    DeviceManager();
    ~DeviceManager();
    std::map<std::string, std::shared_ptr<StreamDock>> enumerate();
    int listen();
    void asyncListen();
    void clearListen();

    std::shared_ptr<TranSport> transport;
    std::map<std::string, std::shared_ptr<StreamDock>> m_deviceMap;
    std::future<void> m_future;
    std::atomic<bool> isListening = false;
};
