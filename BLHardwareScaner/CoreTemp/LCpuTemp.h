/// @file LCpuTemp.h
/// @brief ��ͷ�ļ���������CPU�¶Ȼ�ȡ��
/// 
/// ��ͷ�ļ������������WinRing0, ������Ҫ����ԱȨ��
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 1:9:2015

#ifndef _LCPUTEMP_H_
#define _LCPUTEMP_H_

#include <string>
using std::wstring;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief ����������������
#ifndef MAX_PROCESSOR_PHYSICAL_CORE_NUM
#define MAX_PROCESSOR_PHYSICAL_CORE_NUM 64
#endif

class CCpuTemp;

/// @brief CPU�¶Ƚӿ�
class LCpuTemp
{
public:
    /// @brief ���캯��
    /// @param[in] winRing0Path WinRing0 DLL·��
    explicit LCpuTemp(IN const wstring& winRing0Path);

    /// @brief ��������
    ~LCpuTemp();

    /// @brief ��ȡCPU�¶�
    /// @param[out] coreNum �洢CPU���������
    /// @param[out] temp �洢�¶�, ��λ���϶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Get(OUT unsigned int& coreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]);

private:
    CCpuTemp* m_pCpuTemp;
};

#endif