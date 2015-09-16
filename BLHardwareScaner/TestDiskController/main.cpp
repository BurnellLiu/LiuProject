

#include "..\\Src\\DiskController\\LDiskController.h"

int main()
{
    LIDEDiskController diskController(L"\\\\.\\PhysicalDrive0");
    diskController.GetRotationRate();
    return 0;
}