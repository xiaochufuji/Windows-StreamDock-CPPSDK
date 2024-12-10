#pragma once
#include "streamDock.h"
#include "tranSport.h"
#include <iostream>
#include "hidapi.h"
#include <string>
#include <vector>
#include <cstdio>
#include <string>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/hal/interface.h>
#include <filesystem>
#include <fstream>

class StreamDockN3 :public streamDock
{
public:

    StreamDockN3(tranSport* transport, struct hid_device_info* devInfo);
    ~StreamDockN3();
    /*
        @note:��ȡ�豸�Ĺ̼��汾
        @param lenth ���̼��汾�ĳ���
        @return ���ع̼��汾�İ汾�Ŵ��������׵�ַ����������ؿ�
    */
    unsigned char* getFirmVersion(int lenth);

    /*
        @note:�����豸��Ļ������,��������ֵС��0��ֵ�ᱻ����0���������100�ᱻ��100
        @param percent �����ȵ���ֵ1-100
        @return �ɹ�����0�����������-1
    */
    int setBrightness(int percent);

    /*
        @note:�����豸��Ļ�ı���ͼƬ
        @param path ��ͼƬ��·��
        @return �ɹ�����1�����������-1
    */
    int setBackgroundImg(std::string path);

    /*
        @note:�����豸��Ļ�ı���ͼƬ
        @param path ��ͼƬ��·��
        @return �ɹ�����1�����������-1
    */
    int setBackgroundImgData(unsigned char* imagedata);

    /*
       @note:�����豸���͵���Ϣ
       @return �ɹ����ػ�õ������׵�ַ�����������NULL
   */
    unsigned char* read();
    void read(std::vector<unsigned char>& vec);
    /*
       @note:�����豸������ͼ��
       @param path ��ͼƬ��·��
       @return �ɹ�����1�����������-1
   */
    int setKeyImg(std::string path, int key);
    /*
        @note:�����豸��Ļ�İ���ͼƬ
        @param path �����ͼƬ���ݵ�����
        @return �ɹ�����1�����������-1
    */
    int setKeyImgData(unsigned char* imagedata, int key);
};

