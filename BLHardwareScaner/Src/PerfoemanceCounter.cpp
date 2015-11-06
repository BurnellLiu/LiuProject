
#include "PerformanceCounter.h"

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

/// @brief 性能计数器实现类
class CPerformanceCounter
{
public:
    CPerformanceCounter();
    ~CPerformanceCounter();


    /// @brief 获取内存性能
    /// @param[out] memoryPerformance 存储内存性能
    /// @return 成功返回true, 失败返回false
    bool GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance);

    /// @brief 获取处理器性能
    /// @param[out] processorPerformance 存储处理器性能
    /// @return 成功返回true, 失败返回false
    bool GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance);

private:
    LWMI::LProcessorManager m_processorManager;
    LWMI::LPhysicalMemoryManager m_memoryManager;
    LWMI::LPerfRawData_PerfOS_MemoryManager m_perfMemoryManager;
};

CPerformanceCounter::CPerformanceCounter()
{

}

CPerformanceCounter::~CPerformanceCounter()
{

}

bool CPerformanceCounter::GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance)
{  
    memoryPerformance.TotalSize = 0;
    
    for (int i = 0; i < m_memoryManager.GetPhysicalMemoryCount(); i++)
    {
        unsigned long memorySize = 0;
        if (!m_memoryManager.GetPhysicalMemoryCapacity(i, memorySize))
            return false;
       
        memoryPerformance.TotalSize += memorySize;
    }

    
    if (!m_perfMemoryManager.GetMemoryAvailableMBytes(0, memoryPerformance.AvailableSize))
        return false;

    if (!m_perfMemoryManager.GetMemoryUnusedMBytes(0, memoryPerformance.UnusedSize))
        return false;

    return true;

}

bool CPerformanceCounter::GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance)
{
    LWMI::LProcessorManager processorManager;
    if (!processorManager.GetProcessorLoadPercentage(0, processorPerformance.LoadPercentage))
        return false;

    return true;
}

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