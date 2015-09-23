

#include <Windows.h>


unsigned long GetCPUFrequemcy()
{
    int oldPriority = GetThreadPriority(GetCurrentThread());
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    // 获取计算机的高性能时钟频率(即每秒的滴答数)
    LONGLONG performanceFreq; ///< CPU时钟频率(即每秒钟的滴答数)
    QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFreq);

    LONGLONG startTime; ///< 计时器开始时间 
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

    unsigned __int64 cyclesOnStart = ReadTimeStampCounter();

    SetThreadPriority(GetCurrentThread(), oldPriority);
}

int main()
{
    return 0;
}