

#include "..\\Src\\WinRing0\\LWinRing0.h"

#include <cstdio>

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

//     wchar_t dir[256] = {0};
//     GetCurrentDirectoryW(256, dir);
//     wprintf(L"%s\n", dir);
// 
//     wstring strDir = dir;
//     strDir.append(L"\\..\\Bin");
//     SetCurrentDirectoryW(strDir.c_str());
//     GetCurrentDirectoryW(256, dir);
//     wprintf(L"%s\n", dir);
    system("pause");
    return 0;
}