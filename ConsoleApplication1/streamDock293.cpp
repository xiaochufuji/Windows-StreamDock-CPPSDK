#include "streamDock293.h"

streamDock293::streamDock293(tranSport *transport,struct hid_device_info *devInfo):streamDock(transport,devInfo){

}

int streamDock293::transform(int x) {
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

unsigned char *streamDock293::getFirmVersion(int lenth){
    return transport->getInputReport(lenth);
}


int streamDock293::setBrightness(int percent){
    
    return this->transport->setBrightness(percent);
}

int streamDock293::setBackgroundImg(std::string path)
{

    // 读取图像文件
    cv::Mat image = cv::imread(path);
    //std::cout << "path   setBackgroundImg "<<path << "\n";
    if (image.empty()) {
        std::cerr << "Unable to load image:" << path << std::endl;
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


int streamDock293::setBackgroundImgData(unsigned char* imagedata) {
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
   
void streamDock293::read(std::vector<unsigned char>&vec)
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
    

unsigned char * streamDock293::read()
{
    unsigned char *command=new unsigned char(13);
    if(this->transport->read(command,13)!=-1)
        return command;   
    else
        return NULL;
}

int streamDock293::clearIcon(int index)
{
    int key = transform(index);
    return this->transport->keyClear(key);
}

int streamDock293::setKeyImg(std::string path, int key)
{
    //std::cout << path << "\n" << std::endl;
    //key = transform(key);
    //cv::Mat image = cv::imread(path),Image2;
    //if (image.empty()) {
    //    std::cerr << "Unable to load image:" << path << std::endl;
    //    return -1;
    //}


    //// 创建一个新的图像矩阵用于存储旋转后的图像


    ////// 使用flip函数实现180°旋转
    //cv::flip(image, Image2, -1); // -1表示沿x和y轴翻转

    ////// 保存旋转后的图像
    //imwrite("347274857239.jpg", Image2);
    //int res = this->transport->setKeyImg("347274857239.jpg", key);
    //std::remove("347274857239.jpg");
    //return res;
    key = transform(key);
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED); // 加载图像，保留透明通道
    if (image.empty()) {
        std::cerr << "Unable to load image: " << path << std::endl;
        return -1;
    }

    // 检查是否有透明通道（4 通道）
    if (image.channels() == 4) {
        // 分离通道
        std::vector<cv::Mat> channels(4);
        cv::split(image, channels);

        cv::Mat alpha = channels[3]; // Alpha 通道
        cv::Mat imageBGR;

        // 提取 BGR 通道
        std::vector<cv::Mat> bgrChannels = { channels[0], channels[1], channels[2] };
        cv::merge(bgrChannels, imageBGR);

        // 遍历每个像素，处理透明部分
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                uchar alphaValue = alpha.at<uchar>(y, x);

                if (alphaValue < 255) { // 半透明或完全透明
                    cv::Vec3b& pixel = imageBGR.at<cv::Vec3b>(y, x);
                    double blendFactor = alphaValue / 255.0;

                    // 混合黑色与原始颜色
                    pixel[0] = pixel[0] * blendFactor; // Blue
                    pixel[1] = pixel[1] * blendFactor; // Green
                    pixel[2] = pixel[2] * blendFactor; // Red
                }
            }
        }

        image = imageBGR; // 更新为处理后的图像
    }

    // 创建一个新的图像矩阵用于存储旋转后的图像
    cv::Mat Image2;
    cv::flip(image, Image2, -1); // -1 表示沿 x 和 y 轴翻转

    // 保存旋转后的图像
    cv::imwrite("347274857239.jpg", Image2);

    // 调用保存的图像路径
    int res = this->transport->setKeyImg("347274857239.jpg", key);

    // 删除临时文件
    std::remove("347274857239.jpg");
    return res;

}

//int streamDock293::setKeyImgData(unsigned char* imagedata, int width, int height, int key)
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

int streamDock293::setKeyImgData(unsigned char* imagedata, int key)
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
        cv::rotate(img, img, cv::ROTATE_180);

        // 存储JPG数据的缓冲区
        std::vector<unsigned char> buffer;
        std::vector<int> compressionParams = { cv::IMWRITE_JPEG_QUALITY, 95 }; // 压缩参数

        // 将RGB图像编码为JPG格式
        bool result = cv::imencode(".jpg", img, buffer, compressionParams);
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