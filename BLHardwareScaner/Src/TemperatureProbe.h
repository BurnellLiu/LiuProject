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

/// @brief ����ȡ�����¶�����
#ifndef MAX_DISKTEMP_NUMBER
#define MAX_DISKTEMP_NUMBER 8
#endif

/// @brief �����¶���Ϣ��
struct DiskTempInforArray
{
    unsigned int Count; ///< �ɹ���ȡ���¶ȵĴ��̵�����
    unsigned int Temp[MAX_DISKTEMP_NUMBER]; ///< �����¶�, ��λ���϶�(C)
    wstring DiskDriveID[MAX_DISKTEMP_NUMBER]; ///< ����������ID, �� L"\\\\.\\PhysicalDrive0"
};

/// @brief ����������������
#ifndef MAX_PROCESSOR_PHYSICAL_CORE_NUM
#define MAX_PROCESSOR_PHYSICAL_CORE_NUM 64
#endif

/// @brief CPU�¶���Ϣ
struct CpuTempInfor
{
    unsigned int CoreNum; ///< CPU���������
    unsigned int CoreTemp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]; ///< CPU�����¶�
};

/// @brief ���GPU�¶ȴ���������
#ifndef MAX_GPU_SENSORS_NUMBER
#define MAX_GPU_SENSORS_NUMBER 15
#endif

/// @brief GPU�¶���Ϣ
struct GpuTempInfor
{
    unsigned int SensorsNum; ///< �¶ȴ���������
    unsigned int Temp[MAX_GPU_SENSORS_NUMBER];
};

class CTemperatureProbe;

/// @brief �¶�̽����
class TemperatureProbe
{
public:
    TemperatureProbe();
    ~TemperatureProbe();

    /// @brief ��ȡCPU�¶�
    /// @param[out] OUT cpuTemp �洢CPU�¶���Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetCpuTemp(OUT CpuTempInfor& cpuTemp);

    /// @brief ��ȡGPU�¶�
    /// @param[out] OUT gpuTemp �洢GPU�¶���Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetGpuTemp(OUT GpuTempInfor& gpuTemp);

    /// @brief ��ȡ�����¶�
    /// @param[out] diskTemp �洢�����¶�
    void GetDiskTemp(OUT DiskTempInforArray& diskTemp);
private:
    CTemperatureProbe* m_pTemperatureProbe; ///< �¶�̽��ʵ�������
};


#endif