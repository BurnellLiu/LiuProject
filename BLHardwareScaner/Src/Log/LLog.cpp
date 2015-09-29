
#include "LLog.h"

#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <ctime>
#include <cstring>

#include <map>
using std::map;

#include <Windows.h>

/// @brief LOG���Խṹ
struct SLogProperty 
{
    FILE* LogFile; ///< �ļ�ָ��
    bool ShowThreadId; ///< ��ʶ�Ƿ���ʾ�߳�Id
    bool ShowTime; ///< ��ʶ�Ƿ���ʾ��ǰʱ��
    bool DelayedWrite; ///< ��ʶ�Ƿ��ӳ�д
};

static SLogProperty s_logProperty = 
{
    0,
    false,
    false,
    true
};

/// @brief ��LOG�����Ӹ�����Ϣ
static void AddAssistInforToLog()
{
    if (s_logProperty.LogFile == 0)
        return;

    if (s_logProperty.ShowThreadId)
    {
        static map<DWORD, DWORD> s_threadIdMap;
        // LOG�����ӵ�ǰ�߳�ID, �����ڶ��߳������ֲ�ͬ�̵߳�LOG
        DWORD dwId = GetCurrentThreadId();
        if (s_threadIdMap.count(dwId) == 0)
        {
            s_threadIdMap[dwId] = 0;
            s_threadIdMap[dwId] = s_threadIdMap.size();
        }
   

        char threadId[64] = {0};
        sprintf_s(threadId, "[Thread %u]", s_threadIdMap[dwId]);
        fprintf_s(s_logProperty.LogFile, threadId);
    }
    

    if (s_logProperty.ShowTime)
    {
        // LOG�����ӵ�ǰʱ��
        time_t t = time(0);
        char szTime[64] = {0}; 
        tm tmTemp;
        localtime_s(&tmTemp, &t);
        strftime(szTime, sizeof(szTime), "[%H:%M:%S]",&tmTemp );
        fprintf_s(s_logProperty.LogFile, szTime);
    }
}

bool LLog::Open(IN const char* szFileName)
{
    if (s_logProperty.LogFile != 0)
    {
        fclose(s_logProperty.LogFile);
        s_logProperty.LogFile = 0;
    }

    errno_t ret = fopen_s(&s_logProperty.LogFile, szFileName, "w");
    if (ret == 0 && s_logProperty.LogFile != 0)
        return true;

    return false;
}

void LLog::Close()
{
    if (s_logProperty.LogFile != 0)
    {
        fclose(s_logProperty.LogFile);
        s_logProperty.LogFile = 0;
    }
}

void LLog::ShowThreadId(IN bool bFlag)
{
    s_logProperty.ShowThreadId = bFlag;
}

void LLog::ShowTime(IN bool bFlag)
{
    s_logProperty.ShowTime = bFlag;
}

void LLog::SetDelayedWrite(IN bool bFlag)
{
    s_logProperty.DelayedWrite = bFlag;
}

void LLog::WriteLineW(IN const wchar_t* szFormat, ...)
{
    if (s_logProperty.LogFile == 0)
        return;

    AddAssistInforToLog();

    va_list args;
    va_start(args, szFormat);
    wchar_t printfBuffer[1024] = {0};
    vswprintf_s(printfBuffer, szFormat, args);
    va_end(args);

    fwprintf_s(s_logProperty.LogFile, printfBuffer);
    fwprintf_s(s_logProperty.LogFile, L"\n");

    if (!s_logProperty.DelayedWrite)
        fflush(s_logProperty.LogFile);
}

void LLog::WriteLineA(IN const char* szFormat, ...)
{
    if (s_logProperty.LogFile == 0)
        return;

    AddAssistInforToLog();

    va_list args;
    va_start(args, szFormat);
    char printfBuffer[1024] = {0};
    vsprintf_s(printfBuffer, szFormat, args);
    va_end(args);

    fprintf_s(s_logProperty.LogFile, printfBuffer);
    fprintf_s(s_logProperty.LogFile, "\n");

    if (!s_logProperty.DelayedWrite)
        fflush(s_logProperty.LogFile);
}