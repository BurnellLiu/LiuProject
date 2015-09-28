
#include "LLog.h"

#include <cstdio>
#include <cstdlib>
#include <stdarg.h>
#include <ctime>
#include <cstring>

#include <Windows.h>

/// @brief LOG属性结构
struct SLogProperty 
{
    FILE* LogFile; ///< 文件指针
};

static SLogProperty s_logProperty = 
{
    0
};

/// @brief 在LOG中增加辅助信息
static void AddAssistInforToLog()
{
    if (s_logProperty.LogFile == 0)
        return;

    DWORD dwId = GetCurrentThreadId();
    char threadId[64] = {0};
    sprintf_s(threadId, "[Thread %u]", dwId);
    fprintf_s(s_logProperty.LogFile, threadId);

    time_t t = time(0);
    char szTime[64] = {0}; 
    tm tmTemp;
    localtime_s(&tmTemp, &t);
    strftime(szTime, sizeof(szTime), "[%H:%M:%S]",&tmTemp );
    fprintf_s(s_logProperty.LogFile, szTime);


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
    fflush(s_logProperty.LogFile);
}