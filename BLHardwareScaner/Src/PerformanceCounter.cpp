
#include "PerformanceCounter.h"

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

#include "Memory\\LMemory.h"

#include "Pdh\\LPdh.h"

/// @brief 性能计数器实现类
class CPerformanceCounter
{
public:
    /// @brief 构造函数
    CPerformanceCounter()
    {

    }

    /// @brief 析构函数
    ~CPerformanceCounter()
    {

    }

    /// @brief 获取内存性能
    /// @param[out] memoryPerformance 存储内存性能
    /// @return 成功返回true, 失败返回false
    bool GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance)
    {
        LMemory memory;
        int loadPercent = memory.GetLoadPercent();
        if (loadPercent == -1)
            return false;

        memoryPerformance.LoadPercentage = (unsigned long)loadPercent;

        int totalSize = memory.GetTotalSize();
        if (totalSize == -1)
            return false;

        memoryPerformance.TotalSize = (unsigned long)totalSize;

        int availSize = memory.GetAvailableSize();
        if (availSize == -1)
            return false;

        memoryPerformance.AvailableSize = (unsigned long)availSize;

        return true;
    }

    /// @brief 获取处理器性能
    /// @param[out] processorPerformance 存储处理器性能
    /// @return 成功返回true, 失败返回false
    bool GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance)
    {
        LWMI::LProcessorManager processorManager;
        if (!processorManager.GetProcessorLoadPercentage(0, processorPerformance.LoadPercentage))
            return false;


        return true;
    }

    /// @brief 获取磁盘性能
    /// @param[in] diskPerformance 存储磁盘性能
    /// @return 成功返回true, 失败返回false
    bool GetDiskPerformance(OUT DiskPerformance& diskPerformance)
    {
        unsigned int fixedDiskCount = 0;

        LWMI::LDiskDriveManager diskDriveManager;

        for (int i = 0; i < diskDriveManager.GetDiskCount(); i++)
        {
            LWMI::LDiskDriveManager::LDISK_TYPE diskType;
            diskDriveManager.GetDiskType(i, diskType);
            if (diskType != LWMI::LDiskDriveManager::FIXED_DISK)
            {
                continue;
            }

            wstring deviceId; 
            diskDriveManager.GetDiskDeviceID(i, deviceId);
            diskPerformance.DiskDriveID[fixedDiskCount] = deviceId;
            
            wstring logicalName;
            diskDriveManager.GetDiskLogicalName(i, logicalName);

            wchar_t idIndex = deviceId[deviceId.length()-1];
            for (unsigned int j = 0; j < logicalName.length(); j++)
            {
                if (L';' == logicalName[j])
                    logicalName[j] = L' ';
            }

            // Path Sample: "\\PhysicalDisk(0 C: D:)\\% Disk Time"
            wstring counterPath = L"\\PhysicalDisk(";
            counterPath += idIndex;
            counterPath += L' ';
            counterPath += logicalName;
            counterPath += L")\\% Disk Time";

            LPdh pdh(counterPath);

            long value = 0;
            pdh.CollectDataLong(200, value);
            diskPerformance.UsageRate[fixedDiskCount] = (unsigned int)value;

            fixedDiskCount++;
        }

        diskPerformance.Count = fixedDiskCount;

        return true;
    }
    
};

PerformanceCounter::PerformanceCounter()
{
    m_pCPerformanceCounter = 0;
    m_pCPerformanceCounter = new CPerformanceCounter();
}

PerformanceCounter::~PerformanceCounter()
{
    if (m_pCPerformanceCounter != 0)
    {
        delete m_pCPerformanceCounter;
        m_pCPerformanceCounter = 0;
    }
}

bool PerformanceCounter::GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance)
{
    return m_pCPerformanceCounter->GetMemoryPerformance(memoryPerformance);
}

bool PerformanceCounter::GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance)
{
    return m_pCPerformanceCounter->GetProcessorPerformance(processorPerformance);
}

bool PerformanceCounter::GetDiskPerformance(OUT DiskPerformance& diskPerformance)
{
    return m_pCPerformanceCounter->GetDiskPerformance(diskPerformance);
}