#include "DeviceManager.h"

DeviceManager * other;

DeviceManager::DeviceManager()
{
    this->transport=new tranSport();
    this->streamDockmaps = new std::map<char*, streamDock*>;
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
    delete this->streamDockmaps;
    delete this->transport;
}

std::map<char *,streamDock *> *DeviceManager::enumerate()
{
    delete this->streamDockmaps;
    this->streamDockmaps = new std::map<char*, streamDock*>;
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
            streamDock *node=NULL;
            if (deviceInfo->serial_number==NULL)
            {
                deviceInfo=deviceInfo->next;
                continue;
            }
            if (std::get<2>(product)==1)
            {
                //增加设备时在这个地方添加一个判断，生成不同的设备类
                node = new streamDock293(this->transport, deviceInfo);
                (*this->streamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            else if (std::get<2>(product) == 3 && deviceInfo->usage == 1) {
                node = new StreamDock293V3(this->transport, deviceInfo);
                (*this->streamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            else if (std::get<2>(product) == 4 && deviceInfo->usage == 1) {
                node = new StreamDockN3(this->transport, deviceInfo);
                //node->id = 4;
                (*this->streamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            else if (std::get<2>(product) == 5 && deviceInfo->usage == 1) {
                node = new StreamDockN4(this->transport, deviceInfo);
                //node->id = 4;
                (*this->streamDockmaps)[deviceInfo->path] = node;
                (*this->deviceType)[std::get<2>(product)] = (*this->deviceType)[std::get<2>(product)] + 1;
            }
            deviceInfo = deviceInfo->next;
            //std::cout << deviceInfo->path <<std::endl;
        }
        //this->transport->freeEnumerate(deviceInfo1);
    }
    return streamDockmaps;
}
