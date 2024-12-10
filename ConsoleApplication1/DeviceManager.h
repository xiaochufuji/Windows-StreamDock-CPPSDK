#ifndef _DEVICEMANAGER_
#define _DEVICEMANAGER_

#include <windows.h>
#include "tranSport.h"
#include "streamDock.h"
#include "productIDs.h"
#include "streamDock293.h"
#include "StreamDockN3.h"
#include "StreamDockN4.h"
#include "StreamDock293V3.h"
#include "hidapi.h"
#include <map>
#include <tuple>
#include <vector>
#include <iostream>
#include <objbase.h> 
#include <devguid.h>
#include <setupapi.h>
#include <tchar.h>
#include <Dbt.h>
#include <stdio.h>


class DeviceManager
{
private:
    struct hid_device_info *deviceInfo;
    HINSTANCE hInstance;
    HWND hwnd;
    const GUID GUID_DEVINTERFACE_USB_DEVICE = { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
public:
    DeviceManager(/* args */);
    ~DeviceManager();
    std::map<char *,streamDock *> *enumerate();

    std::map<char*, streamDock*>* streamDockmaps;
    std::map<std::string, bool>* threadMaps;
    std::map<int, int>* deviceType;
    tranSport *transport;
};

#endif