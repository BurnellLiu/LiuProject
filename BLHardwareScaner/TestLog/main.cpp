
#include <cstdio>
#include <cstdlib>

#include <Windows.h>

#include "..\\Src\\Log\\LLog.h"

int main()
{
    LLog::Open(L"test.txt");

    for (unsigned int i = 0; i < 1025; i++)
    {
        PrintLogW(L"Hello, World! %u", i);
    }
    system("pause");
    return 0;
}