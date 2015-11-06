/// @file Gpu.h
/// @brief ���ļ�������GPU������
/// 
/// Detail: ��ͷ�ļ������������nvapi.lib
/// @author Burnell_Liu Email:burnell_liu@qq.com
/// @version   
/// @date 6:11:2015


#ifndef _GPU_H_
#define _GPU_H_

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
#define MAX_GPU_SENSORS_NUMBER 15
#endif

class CGpu;

/// @brief GPU�ӿ�
class LGpu
{
public:
    LGpu();
    ~LGpu();

    /// @brief ��ȡGPU�¶�
    /// @param[out] sensorsNum �洢GPU�¶ȴ���������
    /// @param[out] temp �洢�¶�, ��λ���϶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetTemperature(OUT unsigned int& sensorsNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]);

private:
    CGpu* m_pCGpu; ///< GPUʵ����
};



#endif