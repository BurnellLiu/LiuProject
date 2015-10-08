
#include <cstdio>
#include <cstdlib>

#include <Windows.h>

#include "..\\Src\\Log\\LLog.h"

int main()
{

    LLog::Open("TestLog");
    LLog::WriteLineA("%%Test %d    %s", 32, L"svb");
    LLog::Close();
    printf("%%ss");
    system("pause");
    return 0;
}