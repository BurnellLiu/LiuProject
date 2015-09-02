

#include "TemperatureProbe.h"

#include "Cpu\\LCpuTemperature.h"
#include "Gpu\\Gpu.h"


TemperatureProbe::TemperatureProbe()
{
    m_pCpuTemperature = new LIntelCpuTemperature();
}

TemperatureProbe::~TemperatureProbe()
{
    delete m_pCpuTemperature;
}

unsigned int TemperatureProbe::GetCpuTemp()
{
    unsigned long temp[MAX_PROCESSOR_CORE_NUMBER] = {0};
    bool bRet = m_pCpuTemperature->Get(temp);

    if (bRet)
        return temp[0];
    else
        return 0;
}

unsigned int TemperatureProbe::GetGpuTemp()
{
    static LNvGpu nvGpu;
    static LAMDGpu amdGpu;

    unsigned int temp = nvGpu.GetTemperature();
    if (temp != 0)
        return temp;

    temp = amdGpu.GetTemperature();
    if (temp != 0)
        return temp;

    return 0;
}