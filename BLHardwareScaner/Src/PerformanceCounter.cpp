
#include "PerformanceCounter.h"

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

#include "Memory\\LMemory.h"

/// @brief ���ܼ�����ʵ����
class CPerformanceCounter
{
public:
    CPerformanceCounter()
    {

    }
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
        if (!m_processorManager.GetProcessorLoadPercentage(0, processorPerformance.LoadPercentage))
            return false;


        return true;
    }

private:
    LWMI::LProcessorManager m_processorManager;
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