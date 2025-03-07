## 推荐使用平台VS2022-17

本项目使用cmake和ninja构建
使用说明：

1. 打开Visual Studio, 然后使用`打开文件夹`选项，注意是打开文件夹而不是打开项目，打开`Windows-StreamDock-CPPSDK`根目录，之后等待vs自行配置构建即可
2. 使用前可以先看一下demo.cpp中的示例代码，然后自行修改。
3. 目前仅支持`293, 293V3, N3V25, N4`，如果你想看你的设备是否支持，请观看`ProductIDs.h`中的pid和vid对。

使用示例：
大部分可用函数都在基类StreamDock.h文件里面，请自行查询

```cpp
int main() {
    std::unique_ptr<DeviceManager> manager(new DeviceManager());
    auto streamDocks = manager->enumerate();
    std::cout << "find " << streamDocks.size() << " device" << "\n";
    manager->asyncListen();
    for (auto it = streamDocks.begin();it != streamDocks.end();it++) {
        auto s = it->second;
        // must open first
        s->open(false);
        s->wakeScreen();
        s->setBrightness(50);
        s->setBackgroundImg("./img/YiFei320.png");
        s->refresh();
    }
}
```