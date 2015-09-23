

#include <Windows.h>


unsigned long GetCPUFrequemcy()
{
    int oldPriority = GetThreadPriority(GetCurrentThread());
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    // ��ȡ������ĸ�����ʱ��Ƶ��(��ÿ��ĵδ���)
    LONGLONG performanceFreq; ///< CPUʱ��Ƶ��(��ÿ���ӵĵδ���)
    QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFreq);

    LONGLONG startTime; ///< ��ʱ����ʼʱ�� 
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

    unsigned __int64 cyclesOnStart = ReadTimeStampCounter();

    SetThreadPriority(GetCurrentThread(), oldPriority);
}

int main()
{
    return 0;
}