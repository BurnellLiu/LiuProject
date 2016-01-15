
#include <cstdio>
#include <Windows.h>

#include "..\\Src\\LDiskSpeed.h"

int main()
{
    LDiskSequenceTest diskTest;
    diskTest.Start(L"D:\\test.bin");
    LDiskSpeedTestState testState = diskTest.GetState();
    while (!testState.TestDone)
    {
        testState = diskTest.GetState();
        printf("Write Speed: %.1fM/s Read Speed: %.1fM/s\r", testState.WriteSpeed, testState.ReadSpeed);  
        Sleep(500);
        //diskTest.Stop();
    }
    printf("\n");
    if (testState.Error != DST_NO_ERROR)
    {
        printf("Error\n");
    }
    system("pause");
    return 0;
}