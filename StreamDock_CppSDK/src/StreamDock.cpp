#include "StreamDock.h"

StreamDock::StreamDock(const std::shared_ptr<TranSport>& transport, struct hid_device_info* devInfo) : info(devInfo)
{
	this->transport = transport;
}

StreamDock::~StreamDock()
{

}

std::vector<unsigned char> StreamDock::getFirmVersion(int length)
{
	std::vector<unsigned char> report;
	report.resize(length);
	if (this->transport->getInputReport(report.data(), length) == -1)
	{
		return std::vector<unsigned char>();
	}
	return report;
}

int StreamDock::open(bool auto_read)
{
	return this->transport->open(this->info.path.c_str(), auto_read);
}

int StreamDock::disconnected()
{
	return this->transport->disconnected();
}

int StreamDock::setBrightness(int percent)
{
	return this->transport->setBrightness(percent);
}

int StreamDock::refresh()
{
	return this->transport->refresh();
}

std::string StreamDock::getPath()
{
	return this->info.path;
}

int StreamDock::wakeScreen()
{
	return this->transport->wakeScreen();
}

int StreamDock::clearIcon(int index)
{
	return this->transport->keyClear(index);
}

int StreamDock::clearAllIcon()
{
	return this->transport->keyAllClear();
}

void StreamDock::setKeyFormat(unsigned int width, unsigned int height)
{
	ky_width = width;
	ky_height = height;
}

void StreamDock::setBgiFormat(unsigned int width, unsigned int height)
{
	bg_width = width;
	bg_height = height;
}

void StreamDock::setRotate(float kyAngle, float bgAngle)
{
	kyRotateAngle = kyAngle;
	bgRotateAngle = bgAngle;
}

void StreamDock::setFlipHorizonal(bool bgFlip, bool keyFlip)
{
	flipBgHorizonal = bgFlip;
	flipKyHorizonal = keyFlip;
}

void StreamDock::setFlipVertical(bool bgFlip, bool keyFlip)
{
	flipBgVertical = bgFlip;
	flipKyVertical = keyFlip;
}		  

std::vector<unsigned char> StreamDock::read()
{
	std::vector<unsigned char> buffer;
	buffer.resize(13);
	if (this->transport->read(buffer.data(), 13) == -1)
	{
		return std::vector<unsigned char>();
	}
	return buffer;
}

void PicHelper::toNativeBackground(fipImage& img, StreamDock* dock)
{
	// conver to rgb
	img.convertTo24Bits();
	// resize
	img.rescale(dock->bg_width, dock->bg_height, FILTER_BILINEAR);
	// rotate
	if (dock->bgRotateAngle != 0.0f)
	{
		img.rotate(dock->bgRotateAngle);
	}
	// flip
	if (dock->flipBgHorizonal)
	{
		img.flipHorizontal();
	}
	if (dock->flipBgVertical)
	{
		img.flipVertical();
	}
}

void PicHelper::toNativeKeyImage(fipImage& img, StreamDock* dock)
{
	// conver to rgb
	img.convertTo24Bits();
	// resize
	img.rescale(dock->ky_width, dock->ky_height, FILTER_BILINEAR);
	// rotate
	if (dock->kyRotateAngle != 0.0f)
	{
		img.rotate(dock->kyRotateAngle);
	}
	// flip
	if (dock->flipKyHorizonal)
	{
		img.flipHorizontal();
	}
	if (dock->flipKyVertical)
	{
		img.flipVertical();
	}
}
