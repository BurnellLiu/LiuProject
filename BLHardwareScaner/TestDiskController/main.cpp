
#include <cstdio>

#include <Windows.h>

#include "..\\Src\\DiskController\\LDiskController.h"

int main()
{
    LIDEDiskController diskController(L"\\\\.\\PhysicalDrive1");
    printf("%d\n", diskController.GetRotationRate());
    system("pause");
    return 0;
}