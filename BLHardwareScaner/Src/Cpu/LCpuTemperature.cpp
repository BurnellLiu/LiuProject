

#include "LCpuTemperature.h"

#include <Windows.h>

#include "..\\WinRing0\\OlsApi.h"
#include "..\\WinRing0\\OlsDef.h"

#pragma comment(lib, "WinRing0.lib")


LIntelCpuTemperature::LIntelCpuTemperature()
{
    m_bInitWinRing0Success = false;

    DWORD dwRet = InitializeOls();
    if (TRUE == dwRet)
        m_bInitWinRing0Success = true;
}

LIntelCpuTemperature::~LIntelCpuTemperature()
{
    if (m_bInitWinRing0Success)
        DeinitializeOls();
}

unsigned int LIntelCpuTemperature::Get()
{
    if (!m_bInitWinRing0Success)
        return 0;

    /*
    DTS( Digital Thermal Senser)方式获取CPU温度，通过读取MSR来实现
    现在Intel处理器CPU中每个都集合了DTS，用来实时监测CPU的温度， 
    当温度达到TCC( Thermal Control Circuit)激活温度时， 将会通过降低电压、主频、风扇调节等形式调节温度， 我们称这个温度为Tjunction
    而我们从MSR（Model Specific Register）读到的温度是距离Tjunction的温差，而不是实际的温度，称之为Delta
    所以实际的温度为：Tjunction- Delta
    并且不同CPU的Tjunction还不一样，可以为85℃、100℃、105℃等, 需要查阅Intel用户手册
    以下为Intel 酷睿CPU温度的读取过程:
    */

    // 使用CPUID指令0获取CPU支持的最大命令数
    // 如果最大命令数小于6, 那么CPU不支持DTS
    DWORD eax = 0;
    DWORD ebx = 0;
    DWORD ecx = 0;
    DWORD edx = 0;

    Cpuid(0, &eax, &ebx, &ecx, &edx);

    DWORD maxCmdNum = eax;
    if (maxCmdNum < 6)
        return 0;

    // 使用CPUID指令6, 查看CPU是否支持DTS
    // eax第一位为1则表示支持DTS, 反之不支持DTS
    eax = 0;
    ebx = 0;
    ecx = 0;
    edx = 0;

    Cpuid(6, &eax, &ebx, &ecx, &edx);

    if (0 == (eax & 1))
        return 0;

    // 使用0xee执行rdmsr指令, 如果exa的第30位为1, 则表示Tjunction为85, 否则为100
    eax = 0;
    edx = 0;

    Rdmsr(0xee, &eax, &edx);
    DWORD tjunction = 0;
    if (1 == (eax&0x20000000))
        tjunction = 85;
    else
        tjunction = 100;

    // 使用0x19c执行rdmsr指令, eax的16:23位表示当前DTS值
    eax = 0;
    edx = 0;
    Rdmsr(0x19c, &eax, &edx);

    DWORD delta = (eax&0x007f0000) >> 16;


    return tjunction - delta;
}