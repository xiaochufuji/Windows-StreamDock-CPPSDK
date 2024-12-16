#include "StreamDock.h"

StreamDock::StreamDock(tranSport *transport,struct hid_device_info  *devInfo){

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


unsigned char *StreamDock::getFirmVersion(int lenth)
{
    return this->transport->getInputReport(lenth);
}

int StreamDock::open()
{
    if(this->transport->open(this->path)==-1)
    {
        return -1;
    }
    return 1;
}

int StreamDock::disconnected()
{
    return this->transport->disconnected();
}

int StreamDock::setBrightness(int percent)
{
    return 0;
}

 int StreamDock::setBackgroundImg(std::string path)
 {
     return 0;
 }

 int StreamDock::setBackgroundImgData(unsigned char* imagedata)
 {
     return 0;
 }


unsigned char * StreamDock::read()
{
    return 0;
}

void StreamDock::read(std::vector<unsigned char>& vec)
{
}

int StreamDock::setKeyImg(std::string path,int key)
{
    return 0;
}

int StreamDock::setKeyImgData(unsigned char* imagedata, int key)
{
    return 0;
}

int StreamDock::clearIcon(int index)
{
    return this->transport->keyClear(index);
}

int StreamDock::clearAllIcon()
{
    return this->transport->keyAllClear();
}

int StreamDock::wakeScreen()
{
    return this->transport->wakeScreen();
}


int StreamDock::refresh()
{
    return this->transport->refresh();
}

char* StreamDock::getPath()
{
    return this->path;
}

void StreamDock::rotate(cv::Mat& img1, cv::Mat& img2)
{
    int rows = img1.rows;
    int cols = img2.cols;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            img2.at<cv::Vec3b>(rows - 1 - y, cols - 1 - x) = img1.at<cv::Vec3b>(y, x);
        }
    }
}

cv::Mat StreamDock::rotate90Clockwise(const cv::Mat& image)
{
    int rows = image.rows;
    int cols = image.cols;

    cv::Mat rotatedImage(cols, rows, image.type());

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            rotatedImage.at<cv::Vec3b>(x, rows - y - 1) = image.at<cv::Vec3b>(y, x);
        }
    }

    return rotatedImage;
}


