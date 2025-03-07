#include "DeviceManager.h"
#include "ProductIDs.h"
#include <iostream>
#include <string>
#include <regex>

#ifdef _WIN32
#include <setupapi.h>
#include <windows.h>
#include <Dbt.h>
#pragma comment(lib, "Setupapi.lib")  // 链接 SetupAPI 库
#elif defined(__linux__)
#elif defined(__APPLE__)
#endif

DeviceManager::DeviceManager()
{
	this->transport.reset(new TranSport());
}

DeviceManager::~DeviceManager()
{
	clearListen();
	if (m_future.valid()) m_future.get();
}

std::map<std::string, std::shared_ptr<StreamDock>> DeviceManager::enumerate()
{
	for (auto device : StreamDockDevices)
	{
		// 查看对应vid, pid有没有对应的设备, 有则返回一个链表
		auto infoList = this->transport->enumerateEx(device.first.first, device.first.second);
		auto deviceInfo = infoList._info;
		while (deviceInfo)
		{
			// add device to map
			if (deviceInfo->interface_number == 0)
			{
				std::cout << "Path:" << deviceInfo->path << std::endl;
				std::cout << "Vendor ID:" << deviceInfo->vendor_id << std::endl;
				std::cout << "Product ID:" << deviceInfo->product_id << std::endl;
				std::wcout << "Serial Number:" << deviceInfo->serial_number << std::endl;
				std::wcout << "Manufacturer:" << deviceInfo->manufacturer_string << std::endl;
				std::wcout << "Product:" << deviceInfo->product_string << std::endl;
				std::cout << "Usage:" << deviceInfo->usage << std::endl;
				std::cout << "Usage Page:" << deviceInfo->usage_page << std::endl;
				std::cout << "Interface Number:" << deviceInfo->interface_number << std::endl;
				std::cout << "Release Number:" << deviceInfo->release_number << std::endl;
				std::cout << "------------------------------" << std::endl;
				switch (device.second)
				{
				case Device293:
				{
					m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293(this->transport, deviceInfo));
					break;
				}
				case Device293s:
				{
					//m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293s(this->transport, deviceInfo));
					break;
				}
				case Device293V3:
				{
					m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293V3(this->transport, deviceInfo));
					break;
				}
				case Device293N3:
				{
					m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDockN3(this->transport, deviceInfo));
					break;
				}
				case Device293N4:
				{
					m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDockN4(this->transport, deviceInfo));
					break;
				}
				default:
				{
					break;
				}
				}
			}
			deviceInfo = deviceInfo->next;
		}
	}
	return m_deviceMap;
}

#ifdef _WIN32
int DeviceManager::listen()
{
	bool use_hid = true;
	// hid guid
	const GUID GUID_DEVINTERFACE_HID = { 0x4d1e55b2, 0xf16f, 0x11cf,{ 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };
	// usb guid
	const GUID GUID_DEVINTERFACE_USB_DEVICE = { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };
	HDEVNOTIFY hDeviceNotify = nullptr;
	auto WindowProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ->LRESULT {
		if (uMsg == WM_DEVICECHANGE)
		{
			DeviceManager* manager = reinterpret_cast<DeviceManager*>(GetProp(hwnd, TEXT("@thiPtr_DeviceManager@")));
			PDEV_BROADCAST_DEVICEINTERFACE pDevIntf;
			switch (wParam)
			{
			case DBT_DEVICEARRIVAL:
			{	// 设备插入
				pDevIntf = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
				if (pDevIntf->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
				{
					// 确定为usb/hid设备插入
					std::string devicePath = pDevIntf->dbcc_name;
					std::cout << "[+] Device Inserted: " << devicePath << std::endl;
					/*
					更优解决方案无hid_device_info
					std::regex vidPidRegex(R"(vid_([0-9A-Fa-f]{4})&pid_([0-9A-Fa-f]{4}))", std::regex_constants::icase);
					std::smatch match;
					int vid = 0;
					int pid = 0;
					if (std::regex_search(devicePath, match, vidPidRegex) && match.size() == 3) {
						int vid = std::stoi(match[1], nullptr, 16);
						int pid = std::stoi(match[2], nullptr, 16);
					}
					if (manager->m_deviceMap.find(devicePath) == manager->m_deviceMap.end())
					{
						// 找出对应的设备类型
						auto _pvPair = std::pair<USBVendorIDs, USBProductIDs>(static_cast<USBVendorIDs>(vid), static_cast<USBProductIDs>(pid));
						auto found = StreamDockDevices.find(_pvPair);
						if (found == StreamDockDevices.end()) break;
						auto deviceType = found->second;
						switch (deviceType)
						{
						case Device293:
						{
							manager->m_deviceMap[std::string(devicePath)] = std::shared_ptr<StreamDock>(new StreamDock293(manager->transport, nullptr));
							break;
						}
						case Device293s:
						{
							//manager->m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293s(this->transport, deviceInfo));
							break;
						}
						case Device293V3:
						{
							manager->m_deviceMap[std::string(devicePath)] = std::shared_ptr<StreamDock>(new StreamDock293V3(manager->transport, nullptr));
							break;
						}
						case Device293N3:
						{
							manager->m_deviceMap[std::string(devicePath)] = std::shared_ptr<StreamDock>(new StreamDockN3(manager->transport, nullptr));
							break;
						}
						case Device293N4:
						{
							manager->m_deviceMap[std::string(devicePath)] = std::shared_ptr<StreamDock>(new StreamDockN4(manager->transport, nullptr));
							break;
						}
						default:
						{
							break;
						}
						}
					}
					*/
					for (auto device : StreamDockDevices)
					{
						// 查看对应vid, pid有没有对应的设备, 有则返回一个链表
						auto infoList = manager->transport->enumerateEx(device.first.first, device.first.second);
						auto deviceInfo = infoList._info;
						while (deviceInfo)
						{
							// add device to map
							if (deviceInfo->interface_number == 0)
							{
								switch (device.second)
								{
								case Device293:
								{
									manager->m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293(manager->transport, deviceInfo));
									break;
								}
								case Device293s:
								{
									//manager->m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293s(manager->transport, deviceInfo));
									break;
								}
								case Device293V3:
								{
									manager->m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDock293V3(manager->transport, deviceInfo));
									break;
								}
								case Device293N3:
								{
									manager->m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDockN3(manager->transport, deviceInfo));
									break;
								}
								case Device293N4:
								{
									manager->m_deviceMap[std::string(deviceInfo->path)] = std::shared_ptr<StreamDock>(new StreamDockN4(manager->transport, deviceInfo));
									break;
								}
								default:
								{
									break;
								}
								}
							}
							deviceInfo = deviceInfo->next;
						}
					}
				}
				break;
			}
			case DBT_DEVICEREMOVECOMPLETE:
			{// 设备移除
				pDevIntf = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
				if (pDevIntf->dbcc_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
				{
					// 确定为usb/hid设备插入
					std::string devicePath = pDevIntf->dbcc_name;
					std::cout << "[-] Device Removed: " << devicePath << std::endl;
					auto found = manager->m_deviceMap.find(devicePath);
					if (found != manager->m_deviceMap.end())
					{
						manager->m_deviceMap.erase(devicePath);
					}
				}
				break;
			}
			default:
			{
				break;
			}
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		};
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = TEXT("USBDeviceListener");
	if (!RegisterClass(&wc)) return 0;

	//HWND hwnd = CreateWindow(TEXT("USBDeviceListener"), TEXT("USB Listener"), WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	//	nullptr, nullptr, wc.hInstance, this);
	// 传递 `this` 指针到 `CreateWindowEx`
	HWND hwnd = CreateWindowEx(0, TEXT("USBDeviceListener"), TEXT("USB Listener"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, wc.hInstance, nullptr);
	SetProp(hwnd, TEXT("@thiPtr_DeviceManager@"), this);

	if (!hwnd) return 0;

	// RegisterForDeviceNotifications
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter = {};
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = use_hid ? GUID_DEVINTERFACE_HID : GUID_DEVINTERFACE_USB_DEVICE;
	hDeviceNotify = RegisterDeviceNotification(hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if (!hDeviceNotify)
	{
		if (hwnd) DestroyWindow(hwnd);
		return 0;
	}

	MSG msg;
	isListening = true;
	while (isListening && GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (hDeviceNotify) UnregisterDeviceNotification(hDeviceNotify);
	if (hwnd) DestroyWindow(hwnd);
	return 1;
}

void DeviceManager::clearListen()
{
	isListening = false;
}

#elif defined(__linux__)
#elif defined(__APPLE__)
#endif

void DeviceManager::asyncListen()
{
	if (!m_future.valid())
	{
		m_future = 
			std::async(std::launch::async, [this]() {
				listen();
				});
	}
}


