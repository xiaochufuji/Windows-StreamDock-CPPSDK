## Recommended Platform: VS2022-17

### This project is built using CMake and Ninja.

#### Usage Instructions：

1. Open Visual Studio, then use the `Open Folder` option (make sure to open the folder, not a project). Open the root directory `Windows-StreamDock-CPPSDK`, and wait for VS to automatically configure the build.
2. Before using the project, you may want to check out the example code in `demo.cpp` and modify it as needed.
3. Currently, only `293, 293V3, N3V25, N4` are supported. If you want to check whether your device is supported, refer to the `ProductIDs.h` file for PID and VID pairs.

#### Example Usage:
Most available functions are in the base class `StreamDock.h`. Please refer to it as needed.

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



