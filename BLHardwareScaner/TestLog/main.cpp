
#include <cstdio>
#include <cstdlib>

#include <Windows.h>

#include "..\\Src\\Log\\LLog.h"

int main()
{
    LLog::Open(L"test.txt");

    for (unsigned int i = 0; i < 1; i++)
    {
        PrintLogW(L"%%Hello!");
        PrintLogA("%%Hello, World");
        PrintLogA("%%ÄãºÃ");
    }

    system("pause");
    return 0;
}