
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
    m_displayCardInfor.Count = 0;
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

const DisplayCardInforArray& HardwareInfor::GetDisplayCardInfor() const
{
    return m_displayCardInfor;
}

const PhysicalMemoryInforArray& HardwareInfor::GetPhysicalMemoryInfor() const
{
    return m_physicalMemoryInfor;
}

void HardwareInfor::Scan()
{
    this->ScanComputerSystemInfor(m_computerSystemInfor);
    this->ScanOperatingSystemInfor(m_operatingSystemInfor);
    this->ScanBaseBoardInfor(m_baseBoardInfor);
    this->ScanProcessorInfor(m_processorInfor);
    this->ScanDisplayCardInfor(m_displayCardInfor);
    this->ScanPhysicalMemoryInfor(m_physicalMemoryInfor);
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
}

void HardwareInfor::ScanProcessorInfor(OUT ProcessorInfor& processorInfor)
{
    LWMI::LProcessorManager processorManager;
    processorManager.GetProcessorName(0, processorInfor.Name);
    processorManager.GetProcessorDesription(0, processorInfor.Description);
    processorManager.GetProcessorManufacturer(0, processorInfor.Manufacturer);
    processorManager.GetProcessorCoresNumber(0, processorInfor.CoresNumber);
    processorManager.GetProcessorLogicalProcessorNumber(0, processorInfor.LogicalProcessorNumber);
}

void HardwareInfor::ScanDisplayCardInfor(OUT DisplayCardInforArray& displayCardInfor)
{
    LSetupDisplayCard displayCard;
    int cardCount = displayCard.GetDevNum();
    if (cardCount < 1)
        return;

    displayCardInfor.Count = (unsigned long)cardCount;

    for (int i = 0; i < cardCount && i < MAX_DISPLAYCARD_NUMBER; i++)
    {
        displayCardInfor.Type[i] = DISPLAY_CARD_UNKNOWN;

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
            displayCardInfor.Type[i] = DISPLAY_CARD_INTERNAL;
        else
            displayCardInfor.Type[i] = DISPLAY_CARD_EXTERNAL;


        LWMI::LVideoControllerManager videoControllerManager(instanceID);
        if (videoControllerManager.GetVideoControllerCount() != 1)
            continue;

        videoControllerManager.GetVideoControllerDescription(0, displayCardInfor.Description[i]);
        videoControllerManager.GetVideoControllerAdapterRAMCapacity(0, displayCardInfor.RAMSize[i]);
    }

}

void HardwareInfor::ScanPhysicalMemoryInfor(OUT PhysicalMemoryInforArray& physicalMemoryInfor)
{
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