
#include <cstdio>

#include <Windows.h>

#include "..\\Src\\SetupApi\\LSetupAPI.h"

int main()
{

    LSetupDisplayCard displayCard;
    
    for (int i = 0; i < displayCard.GetDevNum(); i++)
    {
        wstring desc;
        displayCard.GetDevDesc(i, desc);
        wprintf(L"Desc\t%s\n", desc.c_str());
        
        wstring parentInstanceId;
        displayCard.GetParentInstanceId(i, parentInstanceId);
        wprintf(L"ParentInstanceID\t%s\n", parentInstanceId.c_str());

        wstring friendName;
        displayCard.GetFriendlyName(i, friendName);
        wprintf(L"FriendlyName\t%s\n", friendName.c_str());


        wprintf(L"\n");
    }

    system("pause");
    
    return 0;
}