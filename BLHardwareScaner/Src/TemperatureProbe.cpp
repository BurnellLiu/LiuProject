

#include "TemperatureProbe.h"

#include "Cpu\\LCpuTemperature.h"
#include "Gpu\\Gpu.h"


TemperatureProbe::TemperatureProbe()
{
    m_pCpuTemperature = new LIntelCpuTemperature();
    m_pGpu = new LNvGpu();
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
    return m_pGpu->GetTemperature();
}