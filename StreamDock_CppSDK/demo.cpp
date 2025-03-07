#include "DeviceManager.h"
#include <thread>

void test1(const std::shared_ptr<StreamDock>& s, bool test)
{
    if (!test) return;
    s->setBackgroundImg("./img/YiFei320.png");
    s->refresh();
}

void test2(const std::shared_ptr<StreamDock>& s, bool test)
{
    if (!test) return;
    fipImage img;
    img.load("./img/YiFei.png");
    s->setBackgroundImgData(img.accessPixels(), RgbaFormat::RGBA, 800, 480); // we will rescale to 800 * 480
    s->refresh();
}

void test3(const std::shared_ptr<StreamDock> &s, bool test)
{
    if (!test) return;
    s->setKeyImg("./img/tiga112.png", 3);
    s->refresh();
}

void test4(const std::shared_ptr<StreamDock>& s, bool test)
{
    if (!test) return;
    fipImage img;
    img.load("./img/tiga112.png");
    s->setKeyImgData(img.accessPixels(), 2, RgbaFormat::RGBA, 112, 112);
    s->refresh();
}


int main() {
    std::unique_ptr<DeviceManager> manager(new DeviceManager());
    auto streamDocks = manager->enumerate();
    std::cout << "find " << streamDocks.size() << " device" << "\n";
    manager->asyncListen();
    for (auto it = streamDocks.begin();it != streamDocks.end();it++) {
        auto s = it->second;
        s->open(false);
        s->wakeScreen();
        s->setBrightness(50);
        s->setBackgroundImg("./img/YiFei320.png");
        s->refresh();
    }
}
