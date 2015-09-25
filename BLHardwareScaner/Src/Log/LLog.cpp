
#include "LLog.h"

#include <cstdio>
#include <cstdlib>
 #include <stdarg.h>

static FILE* s_pLogFile = 0; ///< ÎÄ¼þÖ¸Õë

bool LLog::Open(IN const string& strFileName)
{
    if (s_pLogFile != 0)
    {
        fclose(s_pLogFile);
        s_pLogFile = 0;
    }

    errno_t ret = fopen_s(&s_pLogFile, strFileName.c_str(), "w");
    if (ret == 0 && s_pLogFile != 0)
        return true;

    return false;
}

void LLog::Close()
{
    if (s_pLogFile != 0)
    {
        fclose(s_pLogFile);
        s_pLogFile = 0;
    }
}

void LLog::WriteLine(IN const string& strFormat, ...)
{
    if (s_pLogFile == 0)
        return;

    const char* szFormat = strFormat.c_str();

    va_list args;
    va_start(args, szFormat);
    char printfBuffer[1024] = {0};
    vsprintf_s(printfBuffer, szFormat, args);
    va_end(args);

    fprintf_s(s_pLogFile, printfBuffer);
    fprintf_s(s_pLogFile, "\n");
    fflush(s_pLogFile);
}

void LLog::WriteLineW(IN const wstring& strFormat, ...)
{
    if (s_pLogFile == 0)
        return;

    const wchar_t* szFormat = strFormat.c_str();

    va_list args;
    va_start(args, szFormat);
    wchar_t printfBuffer[1024] = {0};
    vswprintf_s(printfBuffer, szFormat, args);
    va_end(args);

    fwprintf_s(s_pLogFile, printfBuffer);
    fwprintf_s(s_pLogFile, L"\n");
    fflush(s_pLogFile);
}