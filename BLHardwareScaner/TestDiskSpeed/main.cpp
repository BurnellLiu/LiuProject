
#include <cstdio>
#include <Windows.h>

#include "LDiskSpeed.h"

int main()
{
    LDisk4KRandomTest diskTest;
    diskTest.Start(L"D:\\test.bin");
    LDiskTestState testState = diskTest.GetState();
    while (!testState.TestDone)
    {
        testState = diskTest.GetState();
        printf("Write Speed: %.1fM/s Read Speed: %.1fM/s\r", testState.WriteSpeed, testState.ReadSpeed);  
        Sleep(500);
    }
    printf("\n");
    system("pause");
    return 0;
}