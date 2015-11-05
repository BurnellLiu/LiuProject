

#include "..\\Src\\WinRing0\\LWinRing0.h"

#include <cstdio>
#include <cstring>

#include <Windows.h>


int main()
{
    bool bRet = LWinRing0::InitializeWinRing0(L".\\WinRing0\\WinRing0.dll");
    if (bRet)
    {
        printf("success\n");
    }
    else
    {
        printf("fail\n");
    }

    LWinRing0::DeinitializeWinRing0();

// 
//     wstring strDir = dir;
//     strDir.append(L"\\..\\Bin");
//     SetCurrentDirectoryW(strDir.c_str());
//     GetCurrentDirectoryW(256, dir);
//     wprintf(L"%s\n", dir);
    system("pause");
    return 0;
}