
#include <stdio.h>

#include <Windows.h>

#include "..\\Src\\TemperatureProbe.h"

int main()
{
    TemperatureProbe tempProbe;
    DiskTempInforArray diskTemp;
    CpuTempInfor cpuTemp;
    GpuTempInfor gpuTemp;
    while (true)
    {
        tempProbe.GetDiskTemp(diskTemp);
        tempProbe.GetCpuTemp(cpuTemp);
        tempProbe.GetGpuTemp(gpuTemp);
        printf("Cpu Temp: %u  Gpu Temp: %u Disk Temp: %u\r", cpuTemp.CoreTemp[0], gpuTemp.Temp[0], diskTemp.Temp[0]);
        Sleep(1000);
    }
    return 0;
}