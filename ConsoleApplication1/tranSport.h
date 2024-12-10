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
    /*
        @note:���豸��ʹ���豸·����
        @param path ��ͼƬ��·��
        @return �ɹ�����1�����������-1
    */
    int open(char *path);
    /*
       @note:��ȡ�豸�Ĺ̼�id
       @param lenth ���̼��ų��ȣ�Ĭ��д512��
       @return �ɹ�����1�����������-1
   */
    unsigned char *getInputReport(int lenth);
    /*
       @note:��ȡ�豸�ķ�����Ϣ
       �����¼�
       byte            ����        ֵ         ����
       data[0]     �ظ��ĸ�ʽ      0x41-A
       data[1]     �ظ��ĸ�ʽ      0x43-C
       data[2]     �ظ��ĸ�ʽ      0x4B-K
       data[3-4]       Ԥ��
       data[5]     �ظ��ĸ�ʽ      0x4F-O
       data[6]     �ظ��ĸ�ʽ      0x4B-K
       data[7-8]       Ԥ��
       data[9]     �����±�        0x01-0x0F
       data[10]     �����¼�       0x00-0x01  0x00 ����̧��0x01��������

       Ӧ����Ϣ
       data[0]     �ظ��ĸ�ʽ      0x41-A
       data[1]     �ظ��ĸ�ʽ      0x43-C
       data[2]     �ظ��ĸ�ʽ      0x4B-K
       data[3-4]       Ԥ��
       data[5]     �ظ��ĸ�ʽ      0x4F-O
       data[6]     �ظ��ĸ�ʽ      0x4B-K
       data[7-8]       Ԥ��
       data[9]     ��ʾӦ��        0x00
       data[10-13]     Ԥ��
       @param data ���������ܷ�����Ϣ��unsigned char ����
       @param lenth �����鳤��
       @return �ɹ�����1�����������-1
   */
    int read(unsigned char *data,unsigned long lenth);
    /*
       @note:���豸�·���Ϣ
       @param data:װ�����ݵ�unsigned char����
       @param lenth�����鳤��
       @return �ɹ�����1�����������-1
   */
    int write(unsigned char *data,unsigned long lenth);
    /*
       @note:��enumerate���ص��豸��Ϣ�б�����
       @param devs ��װ���豸��Ϣ������
       @return �ɹ�����1�����������-1
   */
    void freeEnumerate(hid_device_info *devs);
    /*
       @note:��ȡ�豸��Ϣ�б�
       @param vid���豸vid
       @param pid���豸pid
       @return �ɹ�����1�����������-1
   */
    hid_device_info *enumerate(int vid, int pid);
    /*
       @note:�����豸��Ļ���� 0-100
       @param percent ������ 0-100
       @return �ɹ�����1�����������-1
   */
    int setBrightness(int percent);
    /*
        @note:�����豸��Ļ�ı���ͼ
        @param buffer������BGR���ݵ�unsigned char�����ַ
        @param size�������С
        @return �ɹ�����1�����������-1
    */
    int setBackgroundImg(unsigned char* buffer, const int size);
    
    int setBackgroundImgDualDevice(std::string path);
    /*
     @note:�����豸ĳ��������ͼ��
     @param path��ͼƬ·��
     @param key���������
     @return �ɹ�����1�����������-1
 */
    int setKeyImg(std::string path, int key);

    int setKeyImgDualDevice(std::string path, int key);
    int setKeyImgDataDualDevice(std::string path,int key);
    /*
        @note:�����豸ĳ��������ͼ��
        @param buffer��װ��ͼƬ��Ϣ��unsigned char ����
        @param key���������
        @param width�� ͼƬ����
        @param height��ͼƬ�߶�
        @return �ɹ�����1�����������-1
    */
    int setKeyImgData(/*unsigned char* buffer*/std::vector<unsigned char>buffer, int key, int width, int height);
    /*
        @note:���ĳ��������ͼ��
        @param index ��������ţ�1-15��
        @return �ɹ�����1�����������-1
    */
    int keyClear(int i);
    /*
        @note:������а�����ͼ��
        @return �ɹ�����1�����������-1
    */
    int keyAllClear();
    /*
       @note:������Ļ
       @return �ɹ�����1�����������-1
   */
    int wakeScreen();
    /*
        @note:�����ݴ�����ɺ���������ˢ����ʾ
        @return �ɹ�����1�����������-1
    */
    int refresh();
    /*
        @note:�Ͽ�����
        @return �ɹ�����1�����������-1
    */
    int disconnected();
    /*
        @note:�ر��豸
        @return �ɹ�����1�����������-1
    */
    void close();
};



 
    

#endif