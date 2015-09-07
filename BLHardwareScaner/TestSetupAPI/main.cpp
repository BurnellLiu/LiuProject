
#include "..\\Src\\SetupApi\\LSetupAPI.h"


int main()
{
    LMonitorEDID edid;
    LSetupMonitor monitor;
    monitor.GetEDID(1, edid);
    return 0;
}