
#include "..\\Src\\SetupApi\\LSetupAPI.h"


int main()
{

    LSetupMonitor monitor;
    LMonitorExtendInfor extendInfor;
    monitor.GetExtendInfor(0, extendInfor);
    return 0;
}