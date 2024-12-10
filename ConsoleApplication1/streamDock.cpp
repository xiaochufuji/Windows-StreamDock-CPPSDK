#include "streamDock.h"

streamDock::streamDock(tranSport *transport,struct hid_device_info  *devInfo){

    this->transport=transport;

    this->vendor_id=devInfo->vendor_id;
    this->product_id=devInfo->product_id;
    this->product_string=devInfo->product_string;
    this->path=devInfo->path;
    this->serial_number=devInfo->serial_number;
    this->manufacturer_string=devInfo->manufacturer_string;
    this->release_number=devInfo->release_number;
    //std::cout << path << std::endl;
    this->mtx = new std::mutex();
}


unsigned char *streamDock::getFirmVersion(int lenth)
{
    return this->transport->getInputReport(lenth);
}

int streamDock::open()
{
    if(this->transport->open(this->path)==-1)
    {
        return -1;
    }
    return 1;
}

int streamDock::disconnected()
{
    return this->transport->disconnected();
}

int streamDock::setBrightness(int percent)
{
    return 0;
}

 int streamDock::setBackgroundImg(std::string path)
 {
     return 0;
 }

 int streamDock::setBackgroundImgData(unsigned char* imagedata)
 {
     return 0;
 }


unsigned char * streamDock::read()
{
    return 0;
}

void streamDock::read(std::vector<unsigned char>& vec)
{
}

int streamDock::setKeyImg(std::string path,int key)
{
    return 0;
}

int streamDock::setKeyImgData(unsigned char* imagedata, int key)
{
    return 0;
}

int streamDock::clearIcon(int index)
{
    return this->transport->keyClear(index);
}

int streamDock::clearAllIcon()
{
    return this->transport->keyAllClear();
}

int streamDock::wakeScreen()
{
    return this->transport->wakeScreen();
}


int streamDock::refresh()
{
    return this->transport->refresh();
}

char* streamDock::getPath()
{
    return this->path;
}


