
#include "LWinRing0.h"

#include <Windows.h>

typedef BOOL (WINAPI *_InitializeOls)();
typedef VOID (WINAPI *_DeinitializeOls)();

namespace LWinRing0
{
    static HMODULE s_hModule = NULL; ///< ¶¯Ì¬¿âÄ£¿é¾ä±ú

    // DLL
    static _InitializeOls InitializeOls = NULL;
    static _DeinitializeOls DeinitializeOls = NULL;

    bool InitializeWinRing0(const wstring& filePath)
    {
        bool bRet = false;

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

        if (InitializeOls() == TRUE)
            bRet = true;
        else
            bRet = false;

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
