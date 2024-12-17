#include "StreamDock293.h"
#include <filesystem>
namespace fs = std::filesystem;

StreamDock293::StreamDock293(tranSport *transport,struct hid_device_info *devInfo):StreamDock(transport,devInfo){
}

int StreamDock293::transform(int x) {
    if (x >= 1 && x <= 5) {
        return x + 10;
    }
    else if (x >= 6 && x <= 10) {
        return x;
    }
    else if (x >= 11 && x <= 15) {
        return x - 10;
    }
}

unsigned char *StreamDock293::getFirmVersion(int lenth){
    return transport->getInputReport(lenth);
}


int StreamDock293::setBrightness(int percent){
    
    return this->transport->setBrightness(percent);
}

int StreamDock293::setBackgroundImg(std::string path)
{

    // 读取图像文件
    cv::Mat image = cv::imread(path);
    //std::cout << "path   setBackgroundImg "<<path << "\n";
    if (image.empty()) {
        std::cerr << "Unable to load image:" << path << std::endl;
        return -1;
    }
    if (image.cols > 800 || image.rows > 480)
    {
        std::cout << "the picture size isn't suitable" << std::endl;
        return -1;
    }
    int  size = image.rows * image.cols * 3;
    unsigned char* buffer = new unsigned char[size];
    // 遍历图像的每个像素

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            // 获取像素点的BGR值

            cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);

            // 提取BGR分量
            buffer[size - y * image.cols * 3 - x * 3 - 3] = pixel[0];
            buffer[size - y * image.cols * 3 - x * 3 - 2] = pixel[1];
            buffer[size - y * image.cols * 3 - x * 3-1] = pixel[2];

        }
    }
    return this->transport->setBackgroundImg(buffer, size);
}


int StreamDock293::setBackgroundImgData(unsigned char* imagedata) {
    //return this->transport->setBackgroundImg(buffer.data(), width * length * 3);
    try
    {
        int height = 480;
        int width = 800;

        // 创建图像
        cv::Mat img(height, width, CV_MAKETYPE(CV_8U, 3), imagedata, static_cast<size_t>(width * 3));

        int  size = img.rows * img.cols * 3;
        unsigned char* buffer = new unsigned char[size];
        // 遍历图像的每个像素

        for (int y = 0; y < img.rows; y++) {
            for (int x = 0; x < img.cols; x++) {
                // 获取像素点的BGR值

                cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);

                // 提取BGR分量
                buffer[size - y * img.cols * 3 - x * 3 - 3] = pixel[0];
                buffer[size - y * img.cols * 3 - x * 3 - 2] = pixel[1];
                buffer[size - y * img.cols * 3 - x * 3 - 1] = pixel[2];

            }
        }
        return this->transport->setBackgroundImg(buffer, size);
    }
    catch (const std::exception&)
    {
        return 0;
        std::cerr << "Failed to encode the image." << std::endl;
    }
}
   
void StreamDock293::read(std::vector<unsigned char>&vec)
{
    vec.clear();
    unsigned char* command = new unsigned char[13];
    //std::cout << "this->transport: " << this->transport << "\n";
    if (this->transport->read(command, 13) != -1)
    {
        //std::cout << "read\n";
        for (int i = 0; i < 13; i++)
        {
            //std::cout << "push\n";
            //std::cout << "command[10]: " << (int)command[10] << "\n";
            vec.push_back((int)command[i]);
        }
    }
    delete command;
}
    

unsigned char * StreamDock293::read()
{
    unsigned char *command=new unsigned char(13);
    if(this->transport->read(command,13)!=-1)
        return command;   
    else
        return NULL;
}

int StreamDock293::clearIcon(int index)
{
    int key = transform(index);
    return this->transport->keyClear(key);
}

int StreamDock293::setKeyImg(std::string path, int key)
{
    key = transform(key);
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR); // 加载图像，保留透明通道
    if (image.empty()) {
        std::cerr << "Unable to load image: " << path << std::endl;
        return -1;
    }
    if (image.cols > 100 || image.rows > 100)
    {
        std::cout << "the picture size isn't suitable" << std::endl;
        return -1;
    }
    cv::Mat image2 = image.clone(); 
    rotate(image, image2);
    std::string tempFilePath =  "./tmp_293.jpg";
    if (!cv::imwrite(tempFilePath, image2)) {
        std::cerr << "Failed to write image to " << tempFilePath << std::endl;
        return -1;
    }
    int res = this->transport->setKeyImg(tempFilePath, key);
    if (std::remove(tempFilePath.c_str()) != 0) {
        std::cerr << "Error deleting file " << tempFilePath << std::endl;
    }

    return res;

}

//int StreamDock293::setKeyImgData(unsigned char* imagedata, int width, int height, int key)
//{
//
//    std::cout << "setKeyImgData running\n";
//    cv::Mat img(height, width, CV_MAKETYPE(CV_8U,3), imagedata, static_cast<size_t>(width * 3));
//    std::vector<unsigned char>buffer; // 存储JPG数据的缓冲区
//    std::vector<int> compressionParams = { cv::IMWRITE_JPEG_QUALITY, 95 }; // 压缩参数，默认设置为高质量
//    // 将RGB图像编码为JPG格式
//    bool result = cv::imencode(".jpg", img, buffer, compressionParams);
//    if (!result) {
//        std::cerr << "Failed to encode the image to JPG." << std::endl;
//    }
//
//    return this->transport->setKeyImgdata(buffer, key, width, height);
//}

int StreamDock293::setKeyImgData(unsigned char* imagedata, int key)
{
    try
    {
        key = transform(key);
       // std::cout << "setKeyImgData running\n";
        // 创建图像
        int height = 100;
        int width = 100;
        cv::Mat img(height, width, CV_MAKETYPE(CV_8U, 3), imagedata, static_cast<size_t>(width * 3));

        // 旋转图像 180°
        /*cv::rotate(img, img, cv::ROTATE_180);*/
        cv::Mat img2 = img.clone();
        rotate(img, img2);

        // 存储JPG数据的缓冲区
        std::vector<unsigned char> buffer;
        buffer.reserve(1024 * 50);
        std::vector<int> compressionParams = { cv::IMWRITE_JPEG_QUALITY, 95 }; // 压缩参数

        // 将RGB图像编码为JPG格式
        bool result = cv::imencode(".jpg", img2, buffer, compressionParams);
        if (!result) {
            std::cerr << "Failed to encode the image to JPG." << std::endl;
        }

        return this->transport->setKeyImgData(buffer, key, width, height);
    }
    catch (const std::exception&)
    {
        return 0;
        std::cerr << "Failed to encode the image." << std::endl;
    }
}