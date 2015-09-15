
#include "HardwareInfor.h"

#include <algorithm>
using std::transform;

#include "Wmi/LWMISystemClasses.h"
#include "Wmi/LWMIHardwareClasses.h"

#include "SetupApi/LSetupAPI.h"

#include "WifiNetwork/LWifiNetwork.h"

#include "DiskController/LDiskController.h"


/// @brief 将字符串的小写字母转换为大写
///  
/// 不要尝试将非uicode字符串转换大小写, 因为在中文在多字节编码中使用两个字节表示
/// @param[in] str 原始字符串
/// @return 转换后的大写字符串
static wstring WStringToUpper(IN const wstring& str)
{
    wstring newStr = str;
    transform(newStr.begin(), newStr.end(), newStr.begin(), toupper);
    return newStr;
}



HardwareInfor& HardwareInfor::GetInstance()
{
    static HardwareInfor s_instance;
    static bool s_bScanDone = false; // 标识是否完成扫描硬件信息

    if (!s_bScanDone)
    {
        s_instance.Scan();
        s_bScanDone = true;
    }

    return s_instance;
}

HardwareInfor::HardwareInfor()
{
    m_videoCardInfor.Count = 0;
    m_physicalMemoryInfor.Count = 0;
    m_diskInfor.Count = 0;
}

HardwareInfor::~HardwareInfor()
{

}

const ComputerSystemInfor& HardwareInfor::GetComputerSystemInfor() const
{
    return m_computerSystemInfor;
}

const OperatingSystemInfor& HardwareInfor::GetOperatingSystemInfor() const
{
    return m_operatingSystemInfor;
}

const MotherBoardInfor& HardwareInfor::GetMotherBoardInfor() const
{
    return m_motherBoardInfor;
}

const ProcessorInfor& HardwareInfor::GetProcessorInfor() const
{
    return m_processorInfor;
}

const VideoCardInforArray& HardwareInfor::GetVideoCardInfor() const
{
    return m_videoCardInfor;
}

const PhysicalMemoryInforArray& HardwareInfor::GetPhysicalMemoryInfor() const
{
    return m_physicalMemoryInfor;
}

const DiskInforArray& HardwareInfor::GetDiskInfor()
{
    /*
    磁盘信息会发生变化所以需要在每次获取的时候重新扫描
    */
    this->ScanDiskInfor(m_diskInfor);

    return m_diskInfor;
}

const MonitorInforArray& HardwareInfor::GetMonitorInfor()
{
    /*
    显示器信息会发生变化所以需要在每次获取的时候重新扫描
    */
    this->ScanMonitorInfor(m_monitorInfor);

    return m_monitorInfor;
}

const BatteryStaticInfor& HardwareInfor::GetBatteryStaticInfor()
{
    this->ScanBatteryStaticInfor(m_batteryStaticInfor);

    return m_batteryStaticInfor;
}

const NetworkCardInforArray& HardwareInfor::GetNetworkCardInfor()
{
    this->ScanNetworkCardInfor(m_networkCardInfor);

    return m_networkCardInfor;
}

void HardwareInfor::Scan()
{
    this->ScanComputerSystemInfor(m_computerSystemInfor);
    this->ScanOperatingSystemInfor(m_operatingSystemInfor);
    this->ScanMotherBoardInfor(m_motherBoardInfor);
    this->ScanProcessorInfor(m_processorInfor);
    this->ScanVideoCardInfor(m_videoCardInfor);
    this->ScanPhysicalMemoryInfor(m_physicalMemoryInfor);
    this->ScanDiskInfor(m_diskInfor);
    this->ScanMonitorInfor(m_monitorInfor);
    this->ScanBatteryStaticInfor(m_batteryStaticInfor);
    this->ScanNetworkCardInfor(m_networkCardInfor);
}

void HardwareInfor::ScanComputerSystemInfor(OUT ComputerSystemInfor& computerSystemInfor)
{
    computerSystemInfor.Type = COMPUTER_TYPE_UNKNOWN;

    LWMI::LComputerSystemManager computerSytemManager;
    computerSytemManager.GetComputerSystemManufacturer(0, computerSystemInfor.Manufacturer);
    LWMI::LComputerSystemManager::LCOMPUTER_SYSTEM_TYPE pcType;
    computerSytemManager.GetComputerSystemPCType(0, pcType);
    computerSystemInfor.Type = (COMPUTER_TYPE)pcType;
    computerSytemManager.GetComputerSystemModel(0, computerSystemInfor.ModelName);
}

void HardwareInfor::ScanOperatingSystemInfor(OUT OperatingSystemInfor& operatingSystemInfor)
{
    LWMI::LOperatingSystemManager operatingSystemManager;
    operatingSystemManager.GetOSCaption(0, operatingSystemInfor.Caption);
    operatingSystemManager.GetOSArchitecture(0, operatingSystemInfor.Architecture);
    operatingSystemManager.GetOSVersion(0, operatingSystemInfor.Version);
    operatingSystemManager.GetOSSystemDrive(0, operatingSystemInfor.SystemDrive);
}

void HardwareInfor::ScanMotherBoardInfor(OUT MotherBoardInfor& motherBoardInfor)
{
    LWMI::LBaseBoardManager baseBoardManager;
    baseBoardManager.GetBaseBoardManufacturer(0, motherBoardInfor.Manufacturer);
    baseBoardManager.GetBaseBoardSerialNumber(0, motherBoardInfor.SerialNumber);

    LWMI::LBIOSManager biosManager;
    biosManager.GetBIOSSerialNumber(0, motherBoardInfor.BiosSerialNumber);
    biosManager.GetSMBIOSBIOSVersion(0, motherBoardInfor.BiosVersion);
    biosManager.GetBIOSManufacturer(0, motherBoardInfor.BiosVendor);

    LWMI::LMS_SystemInformationManager systemInforManager;
    systemInforManager.GetBaseBoardProductName(0, motherBoardInfor.ProductName);
    systemInforManager.GetBIOSReleaseDate(0, motherBoardInfor.BiosReleaseDate);
}

void HardwareInfor::ScanProcessorInfor(OUT ProcessorInfor& processorInfor)
{
    LWMI::LProcessorManager processorManager;
    processorManager.GetProcessorName(0, processorInfor.Name);
    processorManager.GetProcessorDesription(0, processorInfor.Description);
    processorManager.GetProcessorManufacturer(0, processorInfor.Manufacturer);
    processorManager.GetProcessorCoresNumber(0, processorInfor.CoresNumber);
    processorManager.GetProcessorLogicalProcessorNumber(0, processorInfor.LogicalProcessorNumber);
    processorManager.GetProcessorMaxClockSpeed(0, processorInfor.MaxClockSpeed);
}

void HardwareInfor::ScanVideoCardInfor(OUT VideoCardInforArray& videoCardInfor)
{
    videoCardInfor.Count = 0;

    LSetupDisplayCard displayCard;
    int cardCount = displayCard.GetDevNum();
    if (cardCount < 1)
        return;

    videoCardInfor.Count = (unsigned long)cardCount;

    for (int i = 0; i < cardCount && i < MAX_DISPLAYCARD_NUMBER; i++)
    {
        videoCardInfor.Type[i] = DISPLAY_CARD_UNKNOWN;

        wstring instanceID;
        unsigned long dwRet = displayCard.GetInstanceID(i, instanceID);
        if (0 != dwRet)
            continue;

        unsigned int busNumber;
        dwRet = displayCard.GetBusNumber(i, busNumber);
        if (0 != dwRet)
            continue;

        // 独立显卡挂在PCI插槽口上所以总线号不为0
        // 集成显卡挂在PCI上总线好为0
        if (0 == busNumber)
            videoCardInfor.Type[i] = DISPLAY_CARD_INTERNAL;
        else
            videoCardInfor.Type[i] = DISPLAY_CARD_EXTERNAL;


        LWMI::LVideoControllerManager videoControllerManager(instanceID);
        if (videoControllerManager.GetVideoControllerCount() != 1)
            continue;

        videoControllerManager.GetVideoControllerDescription(0, videoCardInfor.Description[i]);
        videoControllerManager.GetVideoControllerAdapterRAMCapacity(0, videoCardInfor.RAMSize[i]);
    }

}

void HardwareInfor::ScanPhysicalMemoryInfor(OUT PhysicalMemoryInforArray& physicalMemoryInfor)
{
    physicalMemoryInfor.Count = 0;

    LWMI::LPhysicalMemoryManager memoryManager;
    physicalMemoryInfor.Count = (unsigned long)memoryManager.GetPhysicalMemoryCount();
    for (int i = 0; i < (int)physicalMemoryInfor.Count && i < MAX_PHYSICAL_MEMORY_NUMBER; i++)
    {
        memoryManager.GetPhysicalMemoryManufacturer(i, physicalMemoryInfor.Manufacturer[i]);
        memoryManager.GetPhysicalMemoryCapacity(i, physicalMemoryInfor.Capacity[i]);
        memoryManager.GetPhysicalMemoryPartNumber(i, physicalMemoryInfor.PartNumbe[i]);
        memoryManager.GetPhysicalMemorySerialNumber(i, physicalMemoryInfor.SerialNumber[i]);
        memoryManager.GetPhysicalMemorySpeed(i, physicalMemoryInfor.Speed[i]);
    }
}

void HardwareInfor::ScanDiskInfor(OUT DiskInforArray& diskInfor)
{
    diskInfor.Count  = 0;
    LWMI::LDiskDriveManager diskDriveManager;
    diskInfor.Count = (unsigned long)diskDriveManager.GetDiskCount();
    for (int i = 0; i < (int)diskInfor.Count && i < MAX_DISK_NUMBER; i++)
    {
        diskDriveManager.GetDiskModel(i, diskInfor.Model[i]);
        diskDriveManager.GetDiskSerialNumber(i, diskInfor.SerialNumber[i]);
        diskDriveManager.GetDiskSize(i, diskInfor.TotalSize[i]);
        diskDriveManager.GetDiskDeviceID(i, diskInfor.DeviceID[i]);
        diskDriveManager.GetDiskPNPDeviceID(i, diskInfor.PNPDeviceID[i]);
        LWMI::LDiskDriveManager::LDISK_TYPE diskType;
        diskDriveManager.GetDiskType(i, diskType);
        diskInfor.DiskType[i] = (DISK_TYPE )diskType;
        diskDriveManager.GetDiskInterfaceType(i, diskInfor.InterfaceType[i]);

        if (diskInfor.InterfaceType[i].compare(L"IDE"))
        {
            LIDEDiskController ideDiskController(diskInfor.DeviceID[i]);
            diskInfor.RotationRate[i] = ideDiskController.GetRotationRate();
        }
        else
        {
            diskInfor.RotationRate[i] = 0;
        }


    }
}

void HardwareInfor::ScanMonitorInfor(OUT MonitorInforArray& monitorInfor)
{
    monitorInfor.Count = 0;
    LSetupMonitor setupMonitor;
    monitorInfor.Count = (unsigned long)setupMonitor.GetDevNum();
    for (unsigned long i = 0; i < monitorInfor.Count && i < MAX_MONITOR_NUMBER; i++)
    {
        LMonitorExtendInfor extendInfor;
        setupMonitor.GetExtendInfor(i, extendInfor);
        monitorInfor.Name[i] = extendInfor.Name;
        monitorInfor.Date[i] = extendInfor.Date;
    }
}

void HardwareInfor::ScanBatteryStaticInfor(OUT BatteryStaticInfor& batteryStaticInfor)
{
    LWMI::LBatteryManager batteryManager;
    LWMI::LBatteryStaticDataManager batteryStaticDataManager;
    LWMI::LBatteryFullCapacityManager batteryFullCapacityManager;

    if (batteryManager.GetBatteryCount() < 1)
    {
        batteryStaticInfor.Exist = false;
    }

    batteryStaticInfor.Exist = true;

    batteryManager.GetBatteryName(0, batteryStaticInfor.Name);
    batteryStaticDataManager.GetBatteryManufacturerName(0, batteryStaticInfor.Manufacturer);
    batteryStaticDataManager.GetBatterySerialNumber(0, batteryStaticInfor.SerialNumber);

    batteryStaticDataManager.GetBatteryDesignedCapacity(0, batteryStaticInfor.DesignedCapacity);
    batteryFullCapacityManager.GetBatteryFullChargedCapacity(0, batteryStaticInfor.FullChargedCapacity);
    batteryManager.GetBatteryDesignVoltage(0, batteryStaticInfor.DesignedVoltage);
}

void HardwareInfor::ScanNetworkCardInfor(OUT NetworkCardInforArray& networkCardInfor)
{
    // 获取无线网卡GUID
    LWifiNetwork wifiNetwork;
    wstring wlanGUID;
    wifiNetwork.GetWLANGUID(wlanGUID);
    wlanGUID = WStringToUpper(wlanGUID);


    LWMI::LNetworkAdapterManager networkCardManager;
    networkCardInfor.Count = 0;

    for (int i = 0; i < networkCardManager.GetNetworkCardCount() && i < MAX_NETWORKCARD_NUMBER; i++)
    {
        wstring pnpDeviceID;
        networkCardManager.GetNetworkCardPNPDeviceID(i, pnpDeviceID);
        pnpDeviceID = WStringToUpper(pnpDeviceID);

        wstring cardGUID;
        networkCardManager.GetNetworkCardGUID(i, cardGUID);
        cardGUID = WStringToUpper(cardGUID);

        if (wcsncmp(pnpDeviceID.c_str(), L"PCI\\", 4) == 0 ||
            wcsncmp(pnpDeviceID.c_str(), L"USB\\", 4) == 0 ||
            wcsncmp(pnpDeviceID.c_str(), L"SD\\", 3) == 0)
        {
            if (cardGUID.compare(wlanGUID) == 0)
                networkCardInfor.Type[networkCardInfor.Count] = WIFI_NETCARD;
            else
                networkCardInfor.Type[networkCardInfor.Count] = ETHERNET_NETCARD;
        }
        else if (wcsncmp(pnpDeviceID.c_str(), L"BTH", 3) == 0)
        {
            networkCardInfor.Type[networkCardInfor.Count] = BLUETOOTH_NETCARD;
        }
        else
        {
            continue;
        }

        networkCardManager.GetNetworkCardName(networkCardInfor.Count, networkCardInfor.Name[i]);
        networkCardManager.GetNetworkCardManufacturer(networkCardInfor.Count, networkCardInfor.Manufacturer[i]);
        networkCardManager.GetNetworkCardMACAddress(networkCardInfor.Count, networkCardInfor.MACAddress[i]);

        networkCardInfor.Count++;
    }
}