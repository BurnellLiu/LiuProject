
#ifndef _CORE_TEMP_H_
#define _CORE_TEMP_H_

#ifdef _EXPORTING_CORETEMP
#define CORETEMP_API_DECLSPEC __declspec(dllexport)
#else
#define CORETEMP_API_DECLSPEC __declspec(dllimport)
#endif

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

/// @brief ���GPU�¶ȴ���������
#ifndef MAX_GPU_SENSORS_NUMBER
#define MAX_GPU_SENSORS_NUMBER 16
#endif

extern "C"
{
    /// @brief ��ȡCPU�¶�
    /// ��Ҫ����ԱȨ��
    /// @param[out] pCoreNum �洢CPU���������
    /// @param[out] temp �洢CPU��������¶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    CORETEMP_API_DECLSPEC bool GetCpuTemp(
        OUT unsigned int* pCoreNum, 
        OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]);

    /// @brief ��ȡGPU�¶�
    /// ֻ�ܻ�ȡ�����Կ����¶�
    /// @param[out] pSensorNum �洢GPU�¶ȴ���������
    /// @param[out] temp �洢�¶�
    /// @return �ɹ�����true, ʧ�ܷ���false
    CORETEMP_API_DECLSPEC bool GetGpuTemp(
        OUT unsigned int* pSensorNum, 
        OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]);
};

#endif