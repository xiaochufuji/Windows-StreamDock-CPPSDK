#include <windows.h>
#include <iphlpapi.h>
#include <iostream>
#include "hidapi.h"
#include "streamDock.h"
#include "tranSport.h"
#include "streamDock293.h"
#include "DeviceManager.h"
#include <thread>
#include <vector>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "iphlpapi.lib") // ���� IP Helper ��
#define CMD_RESET 0xAA000000

void fun(streamDock* s)
{
    while (1)
    {
        unsigned char* buf = s->read();
        std::cout << buf << "  " << buf + 5 << " ";
        std::cout << (int)buf[9] << " ";
        std::cout << (int)buf[10] << "\n";
    }
}


int main() {
    DeviceManager* manager = new DeviceManager();
    auto streamDocks = manager->enumerate();
    std::cout << "find" << streamDocks->size() << "device" << "\n";

    for (auto it = streamDocks->begin();it != streamDocks->end();it++) {
        auto it1 = streamDocks->begin();
        //��ȡ�豸�����
        auto s = it1->second;
        //���豸
        s->open();
        //�����豸��Ļ
        s->wakeScreen();
        //���̼߳����豸��������Ϣ
        std::thread t1(fun, s);
        //�����豸��Ļ����Ϊ100%
        s->setBrightness(100);
        //������а���ͼ��
        s->clearAllIcon();
        for (int i = 1; i <= 16;++i)
        {
            auto img = cv::imread("12345.png");
            s->setKeyImgData(img.data, i);
            s->refresh();
        }
        Sleep(4000);
        //������а���ͼ��
        s->clearAllIcon();

        t1.join();
    }
}
