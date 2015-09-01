

#include "TemperatureProbe.h"

#include "Cpu\\LCpuTemperature.h"


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
    return m_pCpuTemperature->Get();
}

unsigned int TemperatureProbe::GetGpuTemp()
{
    return 0;
}