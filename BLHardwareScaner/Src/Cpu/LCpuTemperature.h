/// @file LCpuTemperature.h
/// @brief ��ͷ�ļ���������CPU�¶Ȼ�ȡ��
/// 
/// ��ͷ�ļ������������WinRing0, ������Ҫ����ԱȨ��
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 1:9:2015

#ifndef _LCPUTEMPERATURE_H_
#define _LCPUTEMPERATURE_H_

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

class CCpuTemperature;

/// @brief CPU�¶Ƚӿ�
class LCpuTemperature
{
public:
    LCpuTemperature();
    ~LCpuTemperature();

    /// @brief ��ȡGPU�¶�
    /// @param[out] coreNum �洢CPU���������
    /// @param[out] temp �洢�¶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Get(OUT unsigned int& coreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]);

private:
    CCpuTemperature* m_pCpuTemperature;
};

#endif