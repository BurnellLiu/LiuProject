
#include "PerformanceCounter.h"

#include "Wmi\\LWMISystemClasses.h"
#include "Wmi\\LWMIHardwareClasses.h"

PerformanceCounter::PerformanceCounter()
{

}

PerformanceCounter::~PerformanceCounter()
{

}

bool PerformanceCounter::GetMemoryPerformance(OUT MemoryPerformance& memoryPerformance)
{  
    memoryPerformance.TotalSize = 0;
    LWMI::LPhysicalMemoryManager memoryManager;
    for (int i = 0; i < memoryManager.GetPhysicalMemoryCount(); i++)
    {
        unsigned long memorySize = 0;
        if (!memoryManager.GetPhysicalMemoryCapacity(i, memorySize))
            return false;
       
        memoryPerformance.TotalSize += memorySize;
    }

    LWMI::LPerfRawData_PerfOS_MemoryManager perfMemoryManager;
    if (!perfMemoryManager.GetMemoryAvailableMBytes(0, memoryPerformance.AvailableSize))
        return false;

    return true;

}

bool PerformanceCounter::GetProcessorPerformance(OUT ProcessorPerformance& processorPerformance)
{
    LWMI::LProcessorManager processorManager;
    if (!processorManager.GetProcessorLoadPercentage(0, processorPerformance.LoadPercentage))
        return false;

    return true;
}