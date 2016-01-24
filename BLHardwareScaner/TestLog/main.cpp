
#include <cstdio>
#include <cstdlib>
#include <process.h>

#include <Windows.h>

#include "..\\Src\\Log\\LLog.h"

void TestThread1(void* pParam)
{
    for (unsigned int i = 0; i < 100; i++)
    {
        PrintLogW(L"Thread1 Hello!");
        PrintLogA("Thread1 Hello, World");
        PrintLogA("Thread1 ÄãºÃ");
    }
}

void TestThread2(void* pParam)
{
    for (unsigned int i = 0; i < 100; i++)
    {
        PrintLogW(L"Thread2 Hello!");
        PrintLogA("Thread2 Hello, World");
        PrintLogA("Thread2 ÄãºÃ");
    }
}


int main()
{
    LLog::Open(L"test.txt");

    _beginthread(TestThread1, 0, 0);

    _beginthread(TestThread2, 0, 0);

    system("pause");
    return 0;
}