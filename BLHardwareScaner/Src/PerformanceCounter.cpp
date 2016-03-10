
#include "PerformanceCounter.h"

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

#include "Memory\\LMemory.h"

/// @brief 性能计数器实现类
class CPerformanceCounter
{
public:
    CPerformanceCounter()
    {

    }
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