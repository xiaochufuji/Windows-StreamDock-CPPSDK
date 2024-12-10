#include "StreamDockN3.h"

StreamDockN3::StreamDockN3(tranSport* transport, struct hid_device_info* devInfo) :streamDock(transport, devInfo) {

}

unsigned char* StreamDockN3::getFirmVersion(int lenth) {
    return transport->getInputReport(lenth);
}


int StreamDockN3::setBrightness(int percent) {

    return this->transport->setBrightness(percent);
}

int StreamDockN3::setBackgroundImg(std::string path)
{

    // ��ȡͼ���ļ�
    cv::Mat image = cv::imread(path), Image2;
    //std::cout << "path   setBackgroundImg "<<path << "\n";
    if (image.empty()) {
        std::cerr << "Unable to load image:" << path << std::endl;
        return -1;
    }
    cv::rotate(image, Image2, cv::ROTATE_90_CLOCKWISE); // ˳ʱ����ת 90 ��

    // ������ת���ͼ��
    cv::imwrite("bg347274857239.jpg", Image2);
    int res = this->transport->setBackgroundImgDualDevice("bg347274857239.jpg");
    std::remove("bg347274857239.jpg");
    return res;
}


int StreamDockN3::setBackgroundImgData(unsigned char* imagedata) {
    //return this->transport->setBackgroundImg(buffer.data(), width * length * 3);
    try
    {
        int height = 240;
        int width = 320;

        // ����ͼ��
        cv::Mat img(height, width, CV_MAKETYPE(CV_8U, 3), imagedata, static_cast<size_t>(width * 3));

        cv::rotate(img, img, cv::ROTATE_90_CLOCKWISE);
        cv::imwrite("293N3BGData347274857239.jpg", img);
        int res = this->transport->setBackgroundImgDualDevice("293N3BGData347274857239.jpg");
        // ɾ����ʱ�ļ�
        std::remove("293N3BGData347274857239.jpg");
        return res;
    }
    catch (const std::exception&)
    {
        return 0;
        std::cerr << "Failed to encode the image." << std::endl;
    }
}

void StreamDockN3::read(std::vector<unsigned char>& vec)
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


unsigned char* StreamDockN3::read()
{
    unsigned char* command = new unsigned char(13);
    if (this->transport->read(command, 13) != -1)
        return command;
    else
        return NULL;
}

int StreamDockN3::setKeyImg(std::string path, int key)
{
    //key = transform(key);
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED); // ����ͼ�񣬱���͸��ͨ��
    if (image.empty()) {
        std::cerr << "Unable to load image: " << path << std::endl;
        return -1;
    }

    // ����Ƿ���͸��ͨ����4 ͨ����
    if (image.channels() == 4) {
        // ����ͨ��
        std::vector<cv::Mat> channels(4);
        cv::split(image, channels);

        cv::Mat alpha = channels[3]; // Alpha ͨ��
        cv::Mat imageBGR;

        // ��ȡ BGR ͨ��
        std::vector<cv::Mat> bgrChannels = { channels[0], channels[1], channels[2] };
        cv::merge(bgrChannels, imageBGR);

        // ����ÿ�����أ�����͸������
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                uchar alphaValue = alpha.at<uchar>(y, x);

                if (alphaValue < 255) { // ��͸������ȫ͸��
                    cv::Vec3b& pixel = imageBGR.at<cv::Vec3b>(y, x);
                    double blendFactor = alphaValue / 255.0;

                    // ��Ϻ�ɫ��ԭʼ��ɫ
                    pixel[0] = pixel[0] * blendFactor; // Blue
                    pixel[1] = pixel[1] * blendFactor; // Green
                    pixel[2] = pixel[2] * blendFactor; // Red
                }
            }
        }

        image = imageBGR; // ����Ϊ������ͼ��
    }

    // ����һ���µ�ͼ��������ڴ洢��ת���ͼ��
    cv::Mat Image2;
    cv::rotate(image, Image2, cv::ROTATE_90_CLOCKWISE); // ˳ʱ����ת 90 ��

    // ������ת���ͼ��
    cv::imwrite("347274857239.jpg", Image2); 

    // ���ñ����ͼ��·��
    int res = this->transport->setKeyImgDualDevice("347274857239.jpg", key);

    // ɾ����ʱ�ļ�
    std::remove("347274857239.jpg");
    return res;
}

int StreamDockN3::setKeyImgData(unsigned char* imagedata, int key)
{
    try
    {
        int height = 64;
        int width = 64;

        // ����ͼ��
        cv::Mat img(height, width, CV_MAKETYPE(CV_8U, 3), imagedata, static_cast<size_t>(width * 3));

        // ˳ʱ����ת 90 ��
        cv::rotate(img, img, cv::ROTATE_90_CLOCKWISE);
        cv::imwrite("Data347274857239.jpg", img);
        int res = this->transport->setKeyImgDataDualDevice("Data347274857239.jpg",key);
        // ɾ����ʱ�ļ�
        std::remove("Data347274857239.jpg");
        return res;
    }
    catch (const std::exception&)
    {
        return 0;
        std::cerr << "Failed to encode the image." << std::endl;
    }
}