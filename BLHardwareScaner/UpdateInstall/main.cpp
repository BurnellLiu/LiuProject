
#include <cstdio>

#include <string>
using std::string;

#include <Windows.h>

#include "unrar.h"

#pragma comment(lib, "unrar.lib")

/// @brief �ر�������
void KillBLHWScaner()
{
    // �ҵ�������
    HWND hWnd = FindWindow(NULL, "BLHWScaner");
    if (NULL == hWnd)
        return;

    // ��ȡ����ID
    DWORD dwProcessID;
    GetWindowThreadProcessId(hWnd, &dwProcessID);

    // �򿪽��̾��
    HANDLE hHandle = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);

    // �رս���
    if (FALSE != TerminateProcess(hHandle, 0))
    {
        WaitForSingleObject(hHandle, INFINITE);
    }

    CloseHandle(hHandle);
    hHandle = NULL;
}

/// @brief ��ѹѹ���ļ�
/// @return �ɹ�����true, ʧ�ܷ���false
bool DecompressionRarFile()
{
    bool bRet = true;

    RAROpenArchiveData archiveData = {0};
    archiveData.ArcName = "..\\Temp\\Update\\BLHWScaner.rar";
    archiveData.OpenMode = RAR_OM_EXTRACT;

    HANDLE hRarFile = RAROpenArchive(&archiveData);
    if (0 == hRarFile)
    {
        bRet = false;
        goto SAFE_EXIT;
    }

    while (true)
    {
        int iRet = 0;
        // ѹ���ļ�ͷ����
        RARHeaderData headerData = {0};
        iRet = RARReadHeader(hRarFile, &headerData);
        if (0 != iRet)
        {
            bRet = false;
            break;
        }

        // ���ļ���ѹ���ϼ�Ŀ¼��
        string fileName = "..\\";

        // ���ѹ���ļ������������, ���޸��ļ�����: �ļ���������N
        if ((0 == strcmp(headerData.FileName, "Update\\UpdateInstall.exe")) ||
            (0 == strcmp(headerData.FileName, "Update\\unrar.dll")))
        {
            fileName +=headerData.FileName;
            fileName += "N";
        }
        else
        {
            fileName += headerData.FileName;
        }


        char fileNameBuffer[260] = {0};
        strcpy_s(fileNameBuffer, fileName.c_str());
        RARProcessFile(hRarFile, RAR_EXTRACT, NULL, fileNameBuffer);
    }



SAFE_EXIT:
    if (hRarFile != NULL)
    {
        RARCloseArchive(hRarFile);
        hRarFile = 0;

        remove("..\\Temp\\Update\\BLHWScaner.rar");
    }

    return bRet;
}

/// @brief ����������
void StartBLHWScaner()
{
    STARTUPINFO si; //һЩ�ر���������  
    memset(&si, 0, sizeof(STARTUPINFO));  
    si.cb = sizeof(STARTUPINFO);  
    si.dwFlags = STARTF_USESHOWWINDOW;  
    si.wShowWindow = SW_SHOW;  

    PROCESS_INFORMATION pi; //�ر��������ý���

    CreateProcess(NULL, "..\\BLHWScaner.exe", NULL, NULL, FALSE, 0, NULL, "..\\", &si, &pi);
}

int main()
{
    printf("Installing New Version...");


    KillBLHWScaner();

    Sleep(200);

    DecompressionRarFile();

    Sleep(200);

    StartBLHWScaner();

    return 0;
}