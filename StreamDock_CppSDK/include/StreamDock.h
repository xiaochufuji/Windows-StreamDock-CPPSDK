#pragma once
#include "TranSport.h"
#include "hidapi.h"
#include <algorithm>
#include <vector>
#include "FreeImagePlus.h"

enum class RgbaFormat { GRAY = 1, RGB = 3, RGBA = 4 };

class StreamDock {
	friend class PicHelper;
public:
	StreamDock(const std::shared_ptr<TranSport>& transport, struct hid_device_info* devInfo);
	virtual ~StreamDock();
	std::vector<unsigned char> getFirmVersion(int length);
	int open(bool auto_read = true);
	int disconnected();
	std::vector<unsigned char> read();
	int setBrightness(int percent);
	int refresh();
	std::string getPath();
	int wakeScreen();
	virtual int clearIcon(int index);
	virtual int clearAllIcon();

    /*  usage:
        s->setBackgroundImg("./img/bg.jpg");
        s->refresh();
    */
	virtual int setBackgroundImg(const std::string &path) = 0;
    /*  usage:
        // if RGBA, set pitch 4; RGB, set 3; GRAY, set 1.
        // you should set the picture's width and height you load from disk, and we will rescale the picture to target width and height
        fipImage img;
        img.load("./img/bg.png");
        s->setBackgroundImgData(img.accessPixels(), RgbaFormat::RGB, 1234, 567); // we will rescale to 800 * 480
        s->refresh();
    */
	virtual int setBackgroundImgData(unsigned char* imagedata, RgbaFormat pitch = RgbaFormat::RGB, unsigned int loadPicWidth = 0, unsigned int loadPicHeight = 0) = 0;
    /*  usage:
        s->setKeyImg("./img/tiga112.png", 8);
        s->refresh();
    */
	virtual int setKeyImg(const std::string &path, int key) = 0;
    /*  usage:
        // if RGBA, set pitch 4; RGB, set 3; GRAY, set 1.
        // you should set the picture's width and height you load from disk, and we will rescale the picture to target width and height
        fipImage img;
        img.load("./img/tiga112.png");
        s->setKeyImgData(img.accessPixels(), 2, RgbaFormat::RGBA, 112, 112);
        s->refresh();
    */
	virtual int setKeyImgData(unsigned char* imagedata, int key, RgbaFormat pitch = RgbaFormat::RGB, unsigned int loadPicWidth = 0, unsigned int loadPicHeight = 0) = 0;

protected:
	void setKeyFormat(unsigned int width, unsigned int height);
	void setBgiFormat(unsigned int width, unsigned int height);
	void setRotate(float kyAngle, float bgAngle);
	void setFlipHorizonal(bool bgFlip, bool keyFlip);
	void setFlipVertical(bool bgFlip, bool keyFlip);


public:
	std::shared_ptr<TranSport> transport;
	hid_device_info_store info;

protected:
	unsigned int bg_width = 800;
	unsigned int bg_height = 480;
	unsigned int ky_width = 100;
	unsigned int ky_height = 100;
	bool flipBgHorizonal = false;
	bool flipBgVertical = false;
	bool flipKyHorizonal = false;
	bool flipKyVertical = false;
	float kyRotateAngle = 0.0f;
	float bgRotateAngle = 0.0f;
};

class PicHelper
{
public:
	static void toNativeBackground(fipImage& img, StreamDock* dock);
	static void toNativeKeyImage(fipImage& img, StreamDock* dock);
};