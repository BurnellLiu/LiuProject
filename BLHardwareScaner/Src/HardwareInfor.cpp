
#include "HardwareInfor.h"

#include "Wmi/LWMISystemClasses.h"
#include "Wmi/LWMIHardwareClasses.h"

#include "SetupApi/LSetupAPI.h"



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

const BaseBoardInfor& HardwareInfor::GetBaseBoardInfor() const
{
    return m_baseBoardInfor;
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

void HardwareInfor::Scan()
{
    this->ScanComputerSystemInfor(m_computerSystemInfor);
    this->ScanOperatingSystemInfor(m_operatingSystemInfor);
    this->ScanBaseBoardInfor(m_baseBoardInfor);
    this->ScanProcessorInfor(m_processorInfor);
    this->ScanVideoCardInfor(m_videoCardInfor);
    this->ScanPhysicalMemoryInfor(m_physicalMemoryInfor);
    this->ScanDiskInfor(m_diskInfor);
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

void HardwareInfor::ScanBaseBoardInfor(OUT BaseBoardInfor& baseBoardInfor)
{
    LWMI::LBaseBoardManager baseBoardManager;
    baseBoardManager.GetBaseBoardDescription(0, baseBoardInfor.Description);
    baseBoardManager.GetBaseBoardManufacturer(0, baseBoardInfor.Manufacturer);
    baseBoardManager.GetBaseBoardSerialNumber(0, baseBoardInfor.SerialNumber);

    LWMI::LBIOSManager biosManager;
    biosManager.GetBIOSSerialNumber(0, baseBoardInfor.BiosSerialNumber);
    biosManager.GetSMBIOSBIOSVersion(0, baseBoardInfor.BiosVersion);
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