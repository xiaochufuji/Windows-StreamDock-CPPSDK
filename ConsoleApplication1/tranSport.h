#ifndef _TRANSPOT_
#define _TRANSPOT_
#undef UNICODE
#undef _UNICODE
#include <iostream>
#include "hidapi.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <vector>

class tranSport
{
private:
    hid_device *handle;
public:
    tranSport();
    int open(char *path);
    unsigned char *getInputReport(int lenth);
    int read(unsigned char *data,unsigned long lenth);
    int write(unsigned char *data,unsigned long lenth);
    void freeEnumerate(hid_device_info *devs);
    hid_device_info *enumerate(int vid, int pid);
    int setBrightness(int percent);
    int setBackgroundImg(unsigned char* buffer, const int size);
    int setBackgroundImgDualDevice(std::string path);
    int setKeyImg(std::string path, int key);
    int setKeyImgDualDevice(std::string path, int key);
    int setKeyImgDataDualDevice(std::string path,int key);
    int setKeyImgData(/*unsigned char* buffer*/std::vector<unsigned char>buffer, int key, int width, int height);
    int keyClear(int i);
    int keyAllClear();
    int wakeScreen();
    int refresh();
    int disconnected();
    void close();
};



 
    

#endif