
#include <cstdio>

#include <string>
using std::string;

#include <Windows.h>

#include "unrar.h"

#pragma comment(lib, "unrar.lib")

int main()
{
    printf("Rar Dll Version: %d\n", RARGetDllVersion());

    RAROpenArchiveData archiveData = {0};
    archiveData.ArcName = "..\\Temp\\Update\\BLHWScaner.rar";
    archiveData.OpenMode = RAR_OM_EXTRACT;

    HANDLE hRarFile = RAROpenArchive(&archiveData);
    if (0 == hRarFile)
        goto SAFE_EXIT;

    int i = 0;

    while (true)
    {
        int iRet = 0;
        // 压缩文件头数据
        RARHeaderData headerData = {0};
        iRet = RARReadHeader(hRarFile, &headerData);
        if (0 != iRet)
            break;

        printf("Extract File: %s\n", headerData.FileName);
        string fileName = "..\\";
        fileName += headerData.FileName;

        char fileNameBuffer[260] = {0};
        strcpy_s(fileNameBuffer, fileName.c_str());
        printf("New File: %s\n", fileNameBuffer);

        iRet = RARProcessFile(hRarFile, RAR_EXTRACT, NULL, fileNameBuffer);
        if (iRet == 0)
            printf("Success\n");
        else
            printf("Fail\n");
    }
    


SAFE_EXIT:
    if (hRarFile != NULL)
    {
        RARCloseArchive(hRarFile);
        hRarFile = 0;
    }

    system("pause");
    return 0;
}