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
#pragma comment(lib, "iphlpapi.lib") // 链接 IP Helper 库
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
        //获取设备类对象
        auto s = it1->second;
        //打开设备
        s->open();
        //唤醒设备屏幕
        s->wakeScreen();
        //打开线程监听设备反馈的信息
        std::thread t1(fun, s);
        //设置设备屏幕亮度为100%
        s->setBrightness(100);
        //清空所有按键图标
        s->clearAllIcon();
        for (int i = 1; i <= 16;++i)
        {
            auto img = cv::imread("12345.png");
            s->setKeyImgData(img.data, i);
            s->refresh();
        }
        Sleep(4000);
        //清空所有按键图标
        s->clearAllIcon();

        t1.join();
    }
}
