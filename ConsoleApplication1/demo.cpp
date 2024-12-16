#include <windows.h>
#include <iphlpapi.h>
#include <iostream>
#include "hidapi.h"
#include "StreamDock.h"
#include "tranSport.h"
#include "StreamDock293.h"
#include "DeviceManager.h"
#include <thread>
#include <vector>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "iphlpapi.lib") // 链接 IP Helper 库
#define CMD_RESET 0xAA000000

void fun(StreamDock* s)
{
    while (1)
    {
        unsigned char* buf = s->read();
        if (!buf)   continue;
        std::cout << buf << "  " << buf + 5 << " ";
        std::cout << (int)buf[9] << " ";
        std::cout << (int)buf[10] << "\n";
    }
}


int main() {
    DeviceManager* manager = new DeviceManager();
    auto streamDocks = manager->enumerate();
    std::cout << "find" << streamDocks->size() << "device" << "\n";
    std::thread t(&DeviceManager::listen, manager);

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

        //设置设备的背景图片
        std::string path = "./img/YiFei.png";
        s->setBackgroundImg(path);
        s->refresh();
        Sleep(2000);
       
        //清空所有按键图标
        s->clearAllIcon();
        s->refresh();

        //设置按键二的图标
        //for (int i = 15; i >= 11; --i)
        //{   // (N4)
        //    //s->setKeyImg("./img/tiga112.png", i);
        //    auto img = cv::imread("./img/YiFei176.png", cv::IMREAD_COLOR);
        //    s->setKeyImgData(img.data, i);
        //    s->refresh();
        //}
        for (int i = 1; i < 6; ++i)
        {
            auto img = cv::imread("./img/tiga.png", cv::IMREAD_COLOR);
            s->setKeyImgData(img.data, i);
            s->refresh();
        }
        Sleep(2000);

        t1.join();
    }
    t.join();
}