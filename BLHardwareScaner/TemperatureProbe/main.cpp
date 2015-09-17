
#include <stdio.h>

#include <Windows.h>

#include "..\\Src\\TemperatureProbe.h"

int main()
{
    TemperatureProbe tempProbe;
    DiskTempInforArray diskTemp;
    CpuTempInfor cpuTemp;
    while (true)
    {
        tempProbe.GetDiskTemp(diskTemp);
        tempProbe.GetCpuTemp(cpuTemp);
        printf("Cpu Temp: %u  Gpu Temp: %u Disk Temp: %u\r", cpuTemp.CoreTemp[0], tempProbe.GetGpuTemp(), diskTemp.Temp[0]);
        Sleep(1000);
    }
    return 0;
}