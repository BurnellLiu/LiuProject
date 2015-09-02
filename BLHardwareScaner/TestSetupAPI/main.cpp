
#include "..\\Src\\SetupApi\\SetupAPI.h"

#include <Windows.h>

int main()
{
    LSetupDisplayCard displaycard;

    printf("Dev Num: %d\n", displaycard.GetDevNum());

    for (int i = 0; i < displaycard.GetDevNum(); i++)
    {
        printf("Dev Index: %d\n", i);
        string devDesc;
        displaycard.GetDevDesc(i, devDesc);
        printf("Dev Desc: %s\n", devDesc.c_str());
        string hardwareId;
        displaycard.GetHardwareId(i, hardwareId);
        printf("HardwareId: %s\n", hardwareId.c_str());
        string friendName;
        displaycard.GetFriendlyName(i, friendName);
        printf("FriendName: %s\n", friendName.c_str());
    }
    system("pause");
    return 0;
}