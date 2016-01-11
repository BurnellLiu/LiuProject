
#include <cstdio>
#include <Windows.h>

#include "LDiskSpeed.h"

int main()
{
    LDiskSequenceTest diskTest;
    diskTest.Start(L"D:\\test.bin");
    LSpeedTestState testState = diskTest.GetState();
    while (!testState.TestDone)
    {
        testState = diskTest.GetState();
        printf("Write Speed: %.1fM/s\n", testState.WriteSpeed);
        Sleep(500);
    }

    printf("\n");
    system("pause");
    return 0;
}