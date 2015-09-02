
#include <stdio.h>

#include <Windows.h>

#include "..\\Src\\TemperatureProbe.h"

int main()
{
    TemperatureProbe tempProbe;
    while (true)
    {
        printf("Cpu Temp: %u  Gpu Temp: %u\r", tempProbe.GetCpuTemp(), tempProbe.GetGpuTemp());
        Sleep(1000);
    }
    return 0;
}