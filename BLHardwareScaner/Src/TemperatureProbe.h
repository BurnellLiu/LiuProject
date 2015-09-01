/// @file TemperatureProbe.h
/// @brief ��ͷ�ļ����������¶�̽����
/// 
/// ��ͷ�ļ������������WinRing0, ������Ҫ����ԱȨ��
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 1:9:2015

#ifndef _TEMPERATUREPROBE_H_
#define _TEMPERATUREPROBE_H_

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class LCpuTemperature;

/// @brief �¶�̽����
class TemperatureProbe
{
public:
    TemperatureProbe();
    ~TemperatureProbe();

    /// @brief ��ȡCPU�¶�
    ///  
    /// ���CPU�����ص�һ�����ĵ��¶�
    /// @return CPU�¶�, 0(��ȡʧ��)
    unsigned int GetCpuTemp();

    /// @brief ��ȡGPU�¶�
    ///  
    /// @return GPU�¶�, 0(��ȡʧ��)
    unsigned int GetGpuTemp();
private:
    LCpuTemperature* m_pCpuTemperature;
};


#endif