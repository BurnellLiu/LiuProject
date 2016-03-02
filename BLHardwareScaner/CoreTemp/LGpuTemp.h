/// @file LGpuTemp.h
/// @brief ���ļ�������GPU�¶Ȼ�ȡ��
/// 
/// Detail: ��ͷ�ļ������������nvapi.lib
/// @author Burnell_Liu Email:burnell_liu@qq.com
/// @version   
/// @date 6:11:2015


#ifndef _LGPUTEMP_H_
#define _LGPUTEMP_H_

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief ���GPU�¶ȴ���������
#ifndef MAX_GPU_SENSORS_NUMBER
#define MAX_GPU_SENSORS_NUMBER 16
#endif

class CGpuTemp;

/// @brief GPU�¶Ƚӿ�
/// ����ֻ�ܻ�ȡ�������Կ����¶�
class LGpuTemp
{
public:
    /// @brief ���캯��
    LGpuTemp();


    /// @brief ��������
    ~LGpuTemp();

    /// @brief ��ȡGPU�¶�
    /// @param[out] sensorsNum �洢GPU�¶ȴ���������
    /// @param[out] temp �洢�¶�, ��λ���϶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool Get(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]);

private:
    CGpuTemp* m_pGpuTemp; ///< GPUʵ����
};



#endif