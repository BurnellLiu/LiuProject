
#include <cstdio>
#include <cstdlib>

#include <Windows.h>

#include "..\\Src\\Log\\LLog.h"

int main()
{

    LLog::Open("TestLog");
    LLog::WriteLineW(L"Test %d    %s", 32, L"svb");
    LLog::WriteLine("Test22 %s", "ad");
    return 0;
}