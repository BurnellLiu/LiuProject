
#include <cstdio>

#include <Windows.h>

#include "..\\Src\\SetupApi\\LSetupAPI.h"

int main()
{

    LSetupMonitor monitor;
    
    for (int i = 0; i < monitor.GetDevNum(); i++)
    {
        LMonitorExtendInfor extendInfor;
        monitor.GetExtendInfor(i, extendInfor);
        printf("Monitor %d:\n", i + 1);
        printf("Name: %s\n", extendInfor.Name.c_str());
        printf("VID: %s\n", extendInfor.VendorID.c_str());
        printf("PID: %s\n", extendInfor.ProductID.c_str());
        printf("Date: %s\n", extendInfor.Date.c_str());
        printf("\n");
    }

    system("pause");
    
    return 0;
}