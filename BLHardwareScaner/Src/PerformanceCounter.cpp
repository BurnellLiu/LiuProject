
#include "PerformanceCounter.h"

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

#include "Memory\\LMemory.h"

#include "Pdh\\LPdh.h"

/// @brief ���ܼ�����ʵ����
class CPerformanceCounter
{
public:
    /// @brief ���캯��
    CPerformanceCounter()
    {

    }

    /// @brief ��������
    ~CPerformanceCounter()
    {

    }

    /// @brief ��ȡ�ڴ�����
    /// @param[out] memoryPerformance �洢�ڴ�����
    /// @return �ɹ�����true, ʧ�ܷ���false
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

    /// @brief ��ȡ����������
    /// @param[out] processorPerformance �洢����������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance)
    {
        LWMI::LProcessorManager processorManager;
        if (!processorManager.GetProcessorLoadPercentage(0, processorPerformance.LoadPercentage))
            return false;


        return true;
    }

    /// @brief ��ȡ��������
    /// @param[in] diskPerformance �洢��������
    /// @return �ɹ�����true, ʧ�ܷ���false
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