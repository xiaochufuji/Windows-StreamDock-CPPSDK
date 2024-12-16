#include "DeviceManager.h"
#include <Windows.h>

DeviceManager* other;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // ��ȡ WebSocketNotifier ָ��
    //WebSocketNotifier* wsNotifier = (WebSocketNotifier*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    Sleep(100);//��ʱ100����ʹ�豸�ܹ�������
    std::vector<std::tuple<int, int, int>> products = {
        {std::make_tuple((int)USBVendorIDs::USB_VID, (int)USBProductIDs::USB_PID_STREAMDOCK_936,1)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293s, (int)USBProductIDs::USB_PID_STREAMDOCK_936s,2)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293V3, (int)USBProductIDs::USB_PID_STREAMDOCK_293V3,3)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293V3, (int)USBProductIDs::USB_PID_STREAMDOCK_293V3EN,3)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293V3, (int)USBProductIDs::USB_PID_STREAMDOCK_293V25,3)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN3, (int)USBProductIDs::USB_PID_STREAMDOCK_N3,4)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN3, (int)USBProductIDs::USB_PID_STREAMDOCK_N3EN,4)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN4, (int)USBProductIDs::USB_PID_STREAMDOCK_N4,5)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN4EN, (int)USBProductIDs::USB_PID_STREAMDOCK_N4EN,5)},
    };
    switch (message)
    {
    case WM_DEVICECHANGE:
        if (wParam == DBT_DEVICEARRIVAL)
        {
            for (const auto& product : products) {
                struct hid_device_info* info = other->transport->enumerate(std::get<0>(product), std::get<1>(product));
                bool flag = false;
                while (info)
                {
                    //�������е����е��豸�࣬������ǰ���ҳ����豸�ǲ����Ѿ����ڵģ����ڵľ������������ڵľʹ���
                    for (auto it = other->StreamDockmaps->begin(); it != other->StreamDockmaps->end(); it++)
                    {
                        if (std::strcmp(info->path, it->first) == 0)
                            flag = true;
                    }
                    if (!flag)
                    {
                        if (std::get<2>(product) == 1)
                        {
                            (*other->StreamDockmaps)[info->path] = new StreamDock293(other->transport, info);
                            //��¼��������
                            (*other->deviceType)[std::get<2>(product)] = (*other->deviceType)[std::get<2>(product)] + 1;
                        }
                        else if (std::get<2>(product) == 3 && info->usage == 1) {
                            (*other->StreamDockmaps)[info->path] = new StreamDock293V3(other->transport, info);
                            //��¼��������
                            (*other->deviceType)[std::get<2>(product)] = (*other->deviceType)[std::get<2>(product)] + 1;
                        }
                        else if (std::get<2>(product) == 4 && info->usage == 1) {
                            (*other->StreamDockmaps)[info->path] = new StreamDockN3(other->transport, info);
                            //��¼��������
                            (*other->deviceType)[std::get<2>(product)] = (*other->deviceType)[std::get<2>(product)] + 1;
                        }
                        else if (std::get<2>(product) == 5 && info->usage == 1) {
                            (*other->StreamDockmaps)[info->path] = new StreamDockN4(other->transport, info);
                            //��¼��������
                            (*other->deviceType)[std::get<2>(product)] = (*other->deviceType)[std::get<2>(product)] + 1;
                        }
                        //this->transport->freeEnumerate()
                        std::cout << "Create Success\n" << std::endl;
                        auto path = info->path;
                        std::string devicePath(path);
                        //std::string encodedPath = WebSocketNotifier::base64_encode(reinterpret_cast<const unsigned char*>(devicePath.c_str()), devicePath.length());
                        //Message msg("deviceDidConnect", encodedPath);
                        //wsNotifier->notifyClients(msg.toJson());
                        // std::cout << wsNotifier->connectedClients.size()<<"\n";
                        break;
                    }
                    info = info->next;
                }
            }
        }
        else if (wParam == DBT_DEVICEREMOVECOMPLETE)
        {
            //������Ϊ�˱���һ�£������е���
            int deviceNum = 0;
            for (const auto& product : products) {
                struct hid_device_info* info = other->transport->enumerate(std::get<0>(product), std::get<1>(product));
                while (info)
                {
                    if (info->usage == 1) {
                        deviceNum++;
                    }
                    info = info->next;
                }
            }
            if (other->StreamDockmaps->size() == deviceNum) {
                return 0;
            }
            for (const auto& product : products) {
                struct hid_device_info* info = other->transport->enumerate(std::get<0>(product), std::get<1>(product));
                struct hid_device_info* infoHead;
                //std::cout << info << std::endl;
                int flag = 0;
                for (auto it = other->StreamDockmaps->begin(); it != other->StreamDockmaps->end(); it++)
                {
                    if (it->second->product_id != std::get<1>(product) || it->second->vendor_id != std::get<0>(product))
                        continue;
                    infoHead = info;
                    //std::cout << std::get<2>(product) << "\n";
                    if (other->deviceType->find(std::get<2>(product))->second <= 0) {
                        break;
                    }
                    flag = 0;
                    while (infoHead)
                    {
                        //std::cout << "path" << it->first << "   " << info->path << std::endl;
                        if (strcmp(it->first, infoHead->path) == 0)
                        {
                            flag = 1;
                        }
                        infoHead = infoHead->next;
                    }
                    if (flag == 0)
                    {
                        auto path = it->first;
                        std::string devicePath(path);
                        //std::cout << "Deletion successful:" << path << std::endl;
                        //std::string encodedPath = WebSocketNotifier::base64_encode(reinterpret_cast<const unsigned char*>(devicePath.c_str()), devicePath.length());
                        //Message msg("deviceDidDisconnect", encodedPath);
                        ////�����пͻ��˷�����Ϣ
                        //wsNotifier->notifyClients(msg.toJson());
                        //ɾ��read�ļ����߳�
                        auto item = other->threadMaps->find(std::string(path));
                        if (item != other->threadMaps->end()) {
                            item->second = true;
                        }
                        (*other->deviceType)[std::get<2>(product)] = (*other->deviceType)[std::get<2>(product)] - 1;
                        other->StreamDockmaps->erase(it);
                        std::cout << "Deletion successful\n" << std::endl;
                        break;
                    }
                }
            }
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
DeviceManager::DeviceManager()
{
    this->transport=new tranSport();
    this->StreamDockmaps = new std::map<char*, StreamDock*>;
    this->threadMaps = new std::map<std::string, bool>;
    this->deviceType = new std::map<int, int>;
    (*this->deviceType)[1] = 0;
    (*this->deviceType)[2] = 0;
    (*this->deviceType)[3] = 0;
    (*this->deviceType)[4] = 0;
    other = this;
}

DeviceManager::~DeviceManager()
{
    delete this->StreamDockmaps;
    delete this->transport;
}

std::map<char *,StreamDock *> *DeviceManager::enumerate()
{
    delete this->StreamDockmaps;
    this->StreamDockmaps = new std::map<char*, StreamDock*>;
    std::vector<std::tuple<int, int, int>> products = {
        {std::make_tuple((int)USBVendorIDs::USB_VID, (int)USBProductIDs::USB_PID_STREAMDOCK_936,1)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293s, (int)USBProductIDs::USB_PID_STREAMDOCK_936s,2)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293V3, (int)USBProductIDs::USB_PID_STREAMDOCK_293V3,3)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293V3, (int)USBProductIDs::USB_PID_STREAMDOCK_293V3EN,3)},
        {std::make_tuple((int)USBVendorIDs::USB_VID293V3, (int)USBProductIDs::USB_PID_STREAMDOCK_293V25,3)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN3, (int)USBProductIDs::USB_PID_STREAMDOCK_N3,4)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN3, (int)USBProductIDs::USB_PID_STREAMDOCK_N3EN,4)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN4, (int)USBProductIDs::USB_PID_STREAMDOCK_N4,5)},
        {std::make_tuple((int)USBVendorIDs::USB_VIDN4EN, (int)USBProductIDs::USB_PID_STREAMDOCK_N4EN,5)},
    };

    for(const auto& product : products){
        //std::cout << "enumerate:" << "\n";
        struct hid_device_info * deviceInfo=this->transport->enumerate(std::get<0>(product),std::get<1>(product));
        struct hid_device_info * deviceInfo1 = deviceInfo;
        
        while (deviceInfo)
        {
            StreamDock *node=NULL;
            if (deviceInfo->serial_number==NULL)
            {
                deviceInfo=deviceInfo->next;
                continue;
            }
            if (std::get<2>(product)==1)
            {
                //�����豸ʱ������ط����һ���жϣ����ɲ�ͬ���豸��
                node = new StreamDock293(this->transport, deviceInfo);
                (*this->StreamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            else if (std::get<2>(product) == 3 && deviceInfo->usage == 1) {
                node = new StreamDock293V3(this->transport, deviceInfo);
                (*this->StreamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            else if (std::get<2>(product) == 4 && deviceInfo->usage == 1) {
                node = new StreamDockN3(this->transport, deviceInfo);
                //node->id = 4;
                (*this->StreamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            else if (std::get<2>(product) == 5 && deviceInfo->usage == 1) {
                node = new StreamDockN4(this->transport, deviceInfo);
                //node->id = 4;
                (*this->StreamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            deviceInfo = deviceInfo->next;
            //std::cout << deviceInfo->path <<std::endl;
        }
        //this->transport->freeEnumerate(deviceInfo1);
    }
    return StreamDockmaps;
}

int DeviceManager::listen()
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SampleClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        L"SampleClass",
        L"Sample Window",
        0,
        0, 0, 0, 0,
        HWND_MESSAGE, // No actual window is created
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd) {
        std::cerr << "Failed to create window." << std::endl;
        return 1;
    }

    // �� WebSocketNotifier ��ָ��洢������������
    // SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&wsNotifier);

    // ע���豸֪ͨ
    DEV_BROADCAST_DEVICEINTERFACE dbdi = {};
    dbdi.dbcc_size = sizeof(dbdi);
    dbdi.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    dbdi.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
    HDEVNOTIFY hDevNotify = RegisterDeviceNotification(hwnd, &dbdi, DEVICE_NOTIFY_WINDOW_HANDLE);
    if (!hDevNotify) {
        std::cerr << "Failed to register for device notifications." << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}