

#include "LCpuTemp.h"

#include <Windows.h>

#include "LWinRing0.h"



/// @brief CPU�¶�ʵ�ֽӿ�
class CCpuTemp
{
public:
    static bool gInitWinRing0Success; ///< ��־�Ƿ�ɹ���ʼ��WinRing0

public:
    /// @brief ����������
    virtual ~CCpuTemp() = 0{}

    /// @brief ��ȡCPU�¶�
    /// @param[out] coreNum �洢CPU���������
    /// @param[out] temp �洢�¶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    virtual bool Get(OUT unsigned int& coreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]) = 0;

protected:
    /// @brief ��ȡ������������
    /// @param[out] physicalCoreNumber ���������
    /// @param[out] logicalProcessorNumber �߼�����������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetProcessorCoreNumber(unsigned long& physicalCoreNumber, unsigned long& logicalProcessorNumber)
    {
        bool bRet = false;
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION processorInforList = NULL;
        DWORD dwRequireSize = 0;
        SYSTEM_INFO systemInfo = {0};
        DWORD listCount = 0;

        BOOL dwRet = GetLogicalProcessorInformation(processorInforList, &dwRequireSize);
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        processorInforList = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)new char[dwRequireSize];

        dwRet = GetLogicalProcessorInformation(processorInforList, &dwRequireSize);
        if (FALSE == dwRet)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        listCount = dwRequireSize/sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        physicalCoreNumber = 0;
        for (unsigned long i = 0; i < listCount; i++)
        {
            if (processorInforList[i].Relationship == RelationProcessorCore)
            {
                physicalCoreNumber += 1;
            }
        }

        // ��ȡ�߼�����������

        GetNativeSystemInfo(&systemInfo);
        logicalProcessorNumber = systemInfo.dwNumberOfProcessors;

        bRet = true;

SAFE_EXIT:
        if (processorInforList != NULL)
        {
            delete processorInforList;
        }

        return bRet;
    }
};

bool CCpuTemp::gInitWinRing0Success = false;

/// @brief IntelCPU�¶���
class CIntelCpuTemp : public CCpuTemp
{
public:
    /// @brief ���캯��
    CIntelCpuTemp()
    {

    }

    /// @brief ��������
    ~CIntelCpuTemp()
    {

    }

    /// @brief ��ȡCPU�¶�
    /// @param[out] coreNum �洢CPU���������
    /// @param[out] temp �洢�¶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    virtual bool Get(OUT unsigned int& coreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM])
    {
        if (!gInitWinRing0Success)
            return false;

        ZeroMemory(temp, sizeof(unsigned int) * MAX_PROCESSOR_PHYSICAL_CORE_NUM);

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

        LWinRing0::Cpuid(0, &eax, &ebx, &ecx, &edx);

        DWORD maxCmdNum = eax;
        if (maxCmdNum < 6)
            return false;

        // ʹ��CPUIDָ��6, �鿴CPU�Ƿ�֧��DTS
        // eax��һλΪ1���ʾ֧��DTS, ��֮��֧��DTS
        eax = 0;
        ebx = 0;
        ecx = 0;
        edx = 0;

        LWinRing0::Cpuid(6, &eax, &ebx, &ecx, &edx);

        if (0 == (eax & 1))
            return false;

        // ʹ��0xeeִ��rdmsrָ��, ���exa�ĵ�30λΪ1, ���ʾTjunctionΪ85, ����Ϊ100
        eax = 0;
        edx = 0;

        LWinRing0::Rdmsr(0xee, &eax, &edx);
        DWORD tjunction = 0;
        if (1 == (eax&0x20000000))
            tjunction = 85;
        else
            tjunction = 100;

        unsigned long processorCoreNum = 0;
        unsigned long logicalProcessorNum = 0;
        if (!this->GetProcessorCoreNumber(processorCoreNum, logicalProcessorNum))
            return false;

        DWORD step =  logicalProcessorNum/processorCoreNum;

        coreNum = processorCoreNum;

        // ʹ��0x19cִ��rdmsrָ��, eax��16:23λ��ʾ��ǰDTSֵ
        // �ֱ��ȡÿ���߼����������¶�
        for (DWORD processorIndex = 0; processorIndex < logicalProcessorNum; processorIndex += step)
        {
            DWORD threadMask = 1;
            threadMask = threadMask << processorIndex;
            DWORD oldMask = SetThreadAffinityMask(GetCurrentThread(), threadMask);
            if (0 == oldMask)
                return false;

            eax = 0;
            edx = 0;
            LWinRing0::Rdmsr(0x19c, &eax, &edx);
            DWORD delta = (eax&0x007f0000) >> 16;
            temp[processorIndex/step] = tjunction - delta;

            SetThreadAffinityMask(GetCurrentThread(), oldMask);
        }

        return true;
    }
};


/// @brief AMDCPU�¶���
class CAMDCpuTemp : public CCpuTemp
{
public:
    /// @brief ���캯��
    CAMDCpuTemp()
    {

    }

    /// @brief ��������
    ~CAMDCpuTemp()
    {

    }

    /// @brief ��ȡCPU�¶�
    /// @param[out] coreNum �洢CPU���������
    /// @param[out] temp �洢�¶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    virtual bool Get(OUT unsigned int& coreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM])
    {
        if (!gInitWinRing0Success)
            return false;

        unsigned long physicalCoreNum = 0;
        unsigned long logicalCoreNum = 0;
        this->GetProcessorCoreNumber(physicalCoreNum, logicalCoreNum);

        coreNum = physicalCoreNum;

        unsigned int coreTemp = 0;

        const WORD PCI_CONFIG_ADDRESS = 0XCF8;
        const WORD PCI_CONFIG_DATA = 0XCFC;

        // ɨ������PCI�豸�ҵ��豸: VID 0x1022 DID 0x141D
        for(unsigned int bus = 0; bus <= 255; bus++)
        {
            for(unsigned int dev = 0; dev < 32; dev++)
            {
                for(unsigned int func = 0; func < 8; func++)
                {
                    DWORD dwAddr=0X80000000+(bus<<16)+(dev<<11)+(func<<8);

                    // ��ȡ�豸ID�ͳ���ID
                    LWinRing0::WriteIoPortDword(PCI_CONFIG_ADDRESS, dwAddr);
                    DWORD dwData = LWinRing0::ReadIoPortDword(PCI_CONFIG_DATA);

                    // ֵΪ0XFFFFFFFF��ʾ�����ڸ�PCI�豸
                    if(dwData==0XFFFFFFFF)
                    {
                        continue;
                    }

                    DWORD vid=dwData&0XFFFF;
                    DWORD did=(dwData>>16)&0XFFFF;

                    if (vid == 0X1022 &&
                        did == 0X141D)
                    {
                        // �Ĵ�����ֵ��8λΪ�¶�ֵ
                        LWinRing0::WriteIoPortDword(PCI_CONFIG_ADDRESS, dwAddr|0XA4);
                        DWORD dwTemp = LWinRing0::ReadIoPortDword(PCI_CONFIG_DATA);
                        dwTemp = dwTemp >> 24;
                        dwTemp = dwTemp & 0XFF;
                        coreTemp = (int)dwTemp;

                        break;

                    }
                }

                if (coreTemp != 0)
                    break;
            }

            if (coreTemp != 0)
                break;
        }

        for (unsigned int i = 0; i < coreNum; i++)
        {
            temp[i] = coreTemp;
        }

        if (coreTemp != 0)
            return true;

        return false;
    }
};

LCpuTemp::LCpuTemp(IN const wstring& winRing0Path)
{
    m_pCpuTemp = NULL;

    if (!CCpuTemp::gInitWinRing0Success)
    {
        CCpuTemp::gInitWinRing0Success = LWinRing0::InitializeWinRing0(winRing0Path);
        if (!CCpuTemp::gInitWinRing0Success)
            return;
    }

    // ��ȡCPU������Ϣ
    char vendorBuffer[20] = {0};
    int iRet = LWinRing0::Cpuid(
        0, 
        (unsigned long*)&vendorBuffer[0], 
        (unsigned long *)&vendorBuffer[4], 
        (unsigned long*)&vendorBuffer[8], 
        (unsigned long*)&vendorBuffer[12]);

    if (0 == iRet)
        return;

    //AMD:      "AuthenticAMD"
    //Intel:    "GenuineIntel"
    //ע�����edx�� ecx��˳����ַ����ж��ߵ�˳���Ƿ���, ���Ե����������Ĵ���
    unsigned long ecx = *((unsigned long *)&vendorBuffer[8]);
    unsigned long edx = *((unsigned long *)&vendorBuffer[12]);
    *((unsigned long *)&vendorBuffer[12]) = ecx;
    *((unsigned long *)&vendorBuffer[8]) = edx;

    if (0 == strcmp((char*)(&vendorBuffer[4]), "GenuineIntel"))
    {
        m_pCpuTemp = new CIntelCpuTemp();
    }
    else if (0 == strcmp((char*)(&vendorBuffer[4]), "AuthenticAMD"))
    {
        m_pCpuTemp = new CAMDCpuTemp();
    }
}

LCpuTemp::~LCpuTemp()
{
    if (m_pCpuTemp != 0)
    {
        delete m_pCpuTemp;
        m_pCpuTemp = 0;
    }
}

bool LCpuTemp::Get(OUT unsigned int& coreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM])
{
    if (NULL == m_pCpuTemp)
        return false;


    return m_pCpuTemp->Get(coreNum, temp);
}

