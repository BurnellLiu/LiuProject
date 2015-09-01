

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
    DTS( Digital Thermal Senser)��ʽ��ȡCPU�¶ȣ�ͨ����ȡMSR��ʵ��
    ����Intel������CPU��ÿ����������DTS������ʵʱ���CPU���¶ȣ� 
    ���¶ȴﵽTCC( Thermal Control Circuit)�����¶�ʱ�� ����ͨ�����͵�ѹ����Ƶ�����ȵ��ڵ���ʽ�����¶ȣ� ���ǳ�����¶�ΪTjunction
    �����Ǵ�MSR��Model Specific Register���������¶��Ǿ���Tjunction���²������ʵ�ʵ��¶ȣ���֮ΪDelta
    ����ʵ�ʵ��¶�Ϊ��Tjunction- Delta
    ���Ҳ�ͬCPU��Tjunction����һ��������Ϊ85�桢100�桢105���, ��Ҫ����Intel�û��ֲ�
    ����ΪIntel ���CPU�¶ȵĶ�ȡ����:
    */

    // ʹ��CPUIDָ��0��ȡCPU֧�ֵ����������
    // ������������С��6, ��ôCPU��֧��DTS
    DWORD eax = 0;
    DWORD ebx = 0;
    DWORD ecx = 0;
    DWORD edx = 0;

    Cpuid(0, &eax, &ebx, &ecx, &edx);

    DWORD maxCmdNum = eax;
    if (maxCmdNum < 6)
        return 0;

    // ʹ��CPUIDָ��6, �鿴CPU�Ƿ�֧��DTS
    // eax��һλΪ1���ʾ֧��DTS, ��֮��֧��DTS
    eax = 0;
    ebx = 0;
    ecx = 0;
    edx = 0;

    Cpuid(6, &eax, &ebx, &ecx, &edx);

    if (0 == (eax & 1))
        return 0;

    // ʹ��0xeeִ��rdmsrָ��, ���exa�ĵ�30λΪ1, ���ʾTjunctionΪ85, ����Ϊ100
    eax = 0;
    edx = 0;

    Rdmsr(0xee, &eax, &edx);
    DWORD tjunction = 0;
    if (1 == (eax&0x20000000))
        tjunction = 85;
    else
        tjunction = 100;

    // ʹ��0x19cִ��rdmsrָ��, eax��16:23λ��ʾ��ǰDTSֵ
    eax = 0;
    edx = 0;
    Rdmsr(0x19c, &eax, &edx);

    DWORD delta = (eax&0x007f0000) >> 16;


    return tjunction - delta;
}