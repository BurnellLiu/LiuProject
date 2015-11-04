
#include "LWinRing0.h"

#include <Windows.h>

typedef BOOL (WINAPI *_InitializeOls)();
typedef VOID (WINAPI *_DeinitializeOls)();

namespace LWinRing0
{
    static HMODULE s_hModule = NULL; ///< 动态库模块句柄

    // DLL
    static _InitializeOls InitializeOls = NULL;
    static _DeinitializeOls DeinitializeOls = NULL;

    bool InitializeWinRing0(const wstring& filePath)
    {
        bool bRet = false;
        wstring oldWorkPath;

        // 获取当前工作路径
        wchar_t* pBuffer = NULL;
        DWORD bufferLen = GetCurrentDirectoryW(0, 0);
        pBuffer = new wchar_t[bufferLen];
        GetCurrentDirectoryW(bufferLen, pBuffer);
        oldWorkPath = pBuffer;
        delete[] pBuffer;
        pBuffer = NULL;

        if (s_hModule != NULL)
        {
            FreeLibrary(s_hModule);
            s_hModule = NULL;
        }

        s_hModule = LoadLibraryW(filePath.c_str());
        if (s_hModule == NULL)
        {
            goto SAFE_EXIT;
            bRet = false;
        }

        InitializeOls = (_InitializeOls)GetProcAddress (s_hModule, "InitializeOls");
        DeinitializeOls =	 (_DeinitializeOls)GetProcAddress (s_hModule, "DeinitializeOls");

        if (InitializeOls == NULL ||
            DeinitializeOls == NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        // 初始化前需要改变当前工作路径, 不然初始化会失败(dll和exe不在同一目录下)
        do 
        {
            wstring::size_type loc = filePath.find_last_of(L"\\");
            if (loc == wstring::npos)
                break;

            wstring path = filePath.substr(0, loc);
            wstring newWorkPath = oldWorkPath;
            newWorkPath.append(L"\\");
            newWorkPath.append(path);
            SetCurrentDirectoryW(newWorkPath.c_str());

            wchar_t dir[256] = {0};
            GetCurrentDirectoryW(256, dir);
            wprintf(L"%s\n", dir);
        } while (0);
        
        
        if (InitializeOls() == TRUE)
            bRet = true;
        else
            bRet = false;

        SetCurrentDirectoryW(oldWorkPath.c_str());

SAFE_EXIT:
        if (!bRet)
        {
            DeinitializeWinRing0();
        }


        return bRet;
    }

    void DeinitializeWinRing0()
    {
        if (DeinitializeOls != NULL)
        {
            DeinitializeOls();
        }


        InitializeOls = NULL;
        DeinitializeOls = NULL;

        if (s_hModule != NULL)
        {
            FreeLibrary(s_hModule);
            s_hModule = NULL;
        }
    }
}
