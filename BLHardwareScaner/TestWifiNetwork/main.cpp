
#include "..\\Src\\WifiNetwork\\LWifiNetwork.h"

#include <cstdio>

#include <Windows.h>

int main()
{
    bool bWLANExist;
    LWifiNetwork wifiNetwork;
    bWLANExist = wifiNetwork.IsWLANExist();

    wstring desc;
    wifiNetwork.GetWLANDesc(desc);

    if (bWLANExist)
    {
        printf("Exist\n");
    }
    else
    {
        printf("Don't Exist\n");
    }

    wprintf(L"WLAN Desc: %s\n", desc.c_str());

    wstring guid;
    wifiNetwork.GetWLANGUID(guid);
    wprintf(L"GUID: %s\n", guid.c_str());

    system("pause");
    return 0;
}