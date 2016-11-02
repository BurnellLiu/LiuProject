
#include "PerformanceCounter.h"

#include <vector>
using std::vector;

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

#include "Memory\\LMemory.h"

#include "Pdh\\LPdh.h"

/// @brief PDH����ID
/// ��ʹ��PDH��ȡ������Ϣʱ, �ýṹ����ȷ��һ������
struct PdhDiskId 
{
    wchar_t IdIndex; ///< ID����, ��0, 1, 2...
    wstring DeviceId; ///< �豸ID, ��"\\\\.\\PhysicalDrive0"
    wstring LogicalName; ///< �߼����̷�������, �����������֮���ÿո����, ��"C: D:"
};

/// @brief ���ܼ�����ʵ����
class CPerformanceCounter
{
public:
    /// @brief ���캯��
    CPerformanceCounter()
    {
        this->ScanFixedDiskId(m_fixedDiskIdList);
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
    /// �÷���ֻ�ܻ�ȡ�̶����̵�����
    /// @param[in] diskPerformance �洢��������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetDiskPerformance(OUT DiskPerformance& diskPerformance)
    {
        diskPerformance.Count = m_fixedDiskIdList.size();
        for (unsigned int i = 0; i < m_fixedDiskIdList.size(); i++)
        {
            diskPerformance.DiskDriveID[i] = m_fixedDiskIdList[i].DeviceId;

            // Path Sample: "\\PhysicalDisk(0 C: D:)\\% Disk Time"
            wstring counterPath = L"\\PhysicalDisk(";
            counterPath += m_fixedDiskIdList[i].IdIndex;
            counterPath += L' ';
            counterPath += m_fixedDiskIdList[i].LogicalName;
            counterPath += L")\\% Disk Time";

            LPdh pdh(counterPath);

            long value = 0;
            pdh.CollectDataLong(200, value);
            diskPerformance.UsageRate[i] = (unsigned int)value;
        }

        return true;
    }

private:
    /// @brief ɨ��̶�����ID
    /// @param[out] idList �洢ID
    void ScanFixedDiskId(OUT vector<PdhDiskId>& idList)
    {
        idList.clear();

        LWMI::LDiskDriveManager diskDriveManager;

        for (int i = 0; i < diskDriveManager.GetDiskCount(); i++)
        {
            LWMI::LDiskDriveManager::LDISK_TYPE diskType;
            diskDriveManager.GetDiskType(i, diskType);
            if (diskType != LWMI::LDiskDriveManager::FIXED_DISK)
            {
                continue;
            }

            
            PdhDiskId pdhDiskId;
            wstring deviceId; 
            diskDriveManager.GetDiskDeviceID(i, deviceId);
            pdhDiskId.DeviceId = deviceId;
            pdhDiskId.IdIndex = deviceId[deviceId.length()-1];

            wstring logicalName;
            diskDriveManager.GetDiskLogicalName(i, logicalName);
            for (unsigned int j = 0; j < logicalName.length(); j++)
            {
                if (L';' == logicalName[j])
                    logicalName[j] = L' ';
            }
            pdhDiskId.LogicalName = logicalName;

            idList.push_back(pdhDiskId);
        }
    }

private:
    vector<PdhDiskId> m_fixedDiskIdList; ///< �̶�����ID�б�
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