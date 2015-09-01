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


/// @brief CPU�¶Ƚӿ�
class LCpuTemperature
{
public:
    /// @brief ��ȡ�¶Ƚӿ�
    ///  
    /// @return �¶�, 0(��ȡʧ��, ��֧��)
    virtual unsigned int Get() = 0;
};

/// @brief IntelCPU�¶���
class LIntelCpuTemperature : public LCpuTemperature
{
public:
    LIntelCpuTemperature();
    ~LIntelCpuTemperature();

    /// @brief ��ȡ�¶Ƚӿ�
    ///  
    /// @return �¶�, 0(��ȡʧ��, ��֧��)
    virtual unsigned int Get();
private:
    bool m_bInitWinRing0Success; ///< ��ʶ�Ƿ��ʼ��WinRing0�ɹ�
};





#endif