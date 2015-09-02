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

// �������������
#define MAX_PROCESSOR_CORE_NUMBER 64 


/// @brief CPU�¶Ƚӿ�
class LCpuTemperature
{
public:
    /// @brief ��ȡ�¶�
    ///  
    /// ÿ�����������ĵ��¶�ֵ��˳��洢��������, �����е�ֵΪ0��ʾû�иô���������
    /// @param[out] temp[MAX_PROCESSOR_CORE_NUMBER] �洢�¶�ֵ(��λ���϶�), 
    /// @return �ɹ�����true, ʧ�ܷ���false
    virtual bool Get(unsigned long temp[MAX_PROCESSOR_CORE_NUMBER]) = 0;

protected:
    /// @brief ��ȡ������������
    /// @param[out] physicalCoreNumber ���������
    /// @param[out] logicalProcessorNumber �߼�����������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetProcessorCoreNumber(unsigned long& physicalCoreNumber, unsigned long& logicalProcessorNumber);
};

/// @brief IntelCPU�¶���
class LIntelCpuTemperature : public LCpuTemperature
{
public:
    LIntelCpuTemperature();
    ~LIntelCpuTemperature();

    /// @brief ��ȡ�¶�
    ///  
    /// ÿ�����������ĵ��¶�ֵ��˳��洢��������, �����е�ֵΪ0��ʾû�иô���������
    /// @param[out] temp[MAX_PROCESSOR_CORE_NUMBER] �洢�¶�ֵ(��λ���϶�), 
    /// @return �ɹ�����true, ʧ�ܷ���false
    virtual bool Get(unsigned long temp[MAX_PROCESSOR_CORE_NUMBER]);
private:
    bool m_bInitWinRing0Success; ///< ��ʶ�Ƿ��ʼ��WinRing0�ɹ�
};





#endif