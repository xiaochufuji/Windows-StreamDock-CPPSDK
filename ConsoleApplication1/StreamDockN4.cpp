#include "StreamDockN4.h"

StreamDockN4::StreamDockN4(tranSport* transport, struct hid_device_info* devInfo) :streamDock(transport, devInfo) {

}

int StreamDockN4::transform(int x) {
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

unsigned char* StreamDockN4::getFirmVersion(int lenth) {
    return transport->getInputReport(lenth);
}


int StreamDockN4::setBrightness(int percent) {
    return this->transport->setBrightness(percent);
}

int StreamDockN4::setBackgroundImg(std::string path)
{
    // 读取图像文件
    cv::Mat image = cv::imread(path), Image2;
    //std::cout << "path   setBackgroundImg "<<path << "\n";
    if (image.empty()) {
        std::cerr << "Unable to load image:" << path << std::endl;
        return -1;
    }
    //使用flip函数实现180°旋转 -1表示沿x和y轴翻转
    cv::flip(image, Image2, -1);

    // 保存旋转后的图像
    cv::imwrite("bg347274857239.jpg", Image2);
    int res = this->transport->setBackgroundImgDualDevice("bg347274857239.jpg");
    std::remove("bg347274857239.jpg");
    return res;
}


int StreamDockN4::setBackgroundImgData(unsigned char* imagedata) {
    //return this->transport->setBackgroundImg(buffer.data(), width * length * 3);
    try
    {
        int height = 480;
        int width = 800;

        // 创建图像
        cv::Mat img(height, width, CV_MAKETYPE(CV_8U, 3), imagedata, static_cast<size_t>(width * 3));

        //使用flip函数实现180°旋转 -1表示沿x和y轴翻转
        cv::flip(img, img, -1);
        cv::imwrite("293N4BGData347274857239.jpg", img);
        int res = this->transport->setBackgroundImgDualDevice("293N4BGData347274857239.jpg");
        // 删除临时文件
        std::remove("293N4BGData347274857239.jpg");
        return res;
    }
    catch (const std::exception&)
    {
        return 0;
        std::cerr << "Failed to encode the image." << std::endl;
    }
}

void StreamDockN4::read(std::vector<unsigned char>& vec)
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


unsigned char* StreamDockN4::read()
{
    unsigned char* command = new unsigned char(13);
    if (this->transport->read(command, 13) != -1)
        return command;
    else
        return NULL;
}

int StreamDockN4::setKeyImg(std::string path, int key)
{
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
    //使用flip函数实现180°旋转 -1表示沿x和y轴翻转
    cv::flip(image, Image2, -1);

    // 保存旋转后的图像
    cv::imwrite("347274857239.jpg", Image2);

    // 调用保存的图像路径
    int res = this->transport->setKeyImgDualDevice("347274857239.jpg", key);

    // 删除临时文件
    std::remove("347274857239.jpg");
    return res;
}

int StreamDockN4::setKeyImgData(unsigned char* imagedata, int key)
{
    try
    {
        key = transform(key);
        int height = 112;
        int width = 112;
        if (key < 5) {
            width = 176;
        }
        // 创建图像
        cv::Mat img(height, width, CV_MAKETYPE(CV_8U, 3), imagedata, static_cast<size_t>(width * 3));

        //使用flip函数实现180°旋转 -1表示沿x和y轴翻转
        cv::flip(img, img, -1);
        cv::imwrite("N4Data347274857239.jpg", img);
        int res = this->transport->setKeyImgDataDualDevice("N4Data347274857239.jpg", key);
        // 删除临时文件
        std::remove("N4Data347274857239.jpg");
        return res;
    }
    catch (const std::exception&)
    {
        return 0;
        std::cerr << "Failed to encode the image." << std::endl;
    }
}

int StreamDockN4::clearIcon(int index)
{
    int key = transform(index);
    return this->transport->keyClear(key);
}
