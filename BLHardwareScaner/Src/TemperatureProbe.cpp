

#include "TemperatureProbe.h"

#include <Windows.h>

#include "Cpu\\LCpuTemperature.h"
#include "Gpu\\Gpu.h"

#include "DiskController\\LDiskController.h"
#include "SMARTPaser\\LSMARTPaser.h"

/// @brief �¶�̽����
class CTemperatureProbe
{
public:
    CTemperatureProbe();
    ~CTemperatureProbe();

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
    LCpuTemperature* m_pCpuTemperature; ///< CPU�¶Ȼ�ȡ�����
    LGpu* m_pGpu; ///< GPU���������
};


CTemperatureProbe::CTemperatureProbe()
{
    m_pCpuTemperature = new LCpuTemperature();
    m_pGpu = new LGpu();
}

CTemperatureProbe::~CTemperatureProbe()
{
    if (m_pCpuTemperature != 0)
    {
        delete m_pCpuTemperature;
        m_pCpuTemperature = 0;
    }
}

bool CTemperatureProbe::GetCpuTemp(OUT CpuTempInfor& cpuTemp)
{
    cpuTemp.CoreNum = 0;

    if (m_pCpuTemperature == 0)
    {
        m_pCpuTemperature = new LCpuTemperature();
    }

    return m_pCpuTemperature->Get(cpuTemp.CoreNum, cpuTemp.CoreTemp);
}

bool CTemperatureProbe::GetGpuTemp(OUT GpuTempInfor& gpuTemp)
{
    gpuTemp.SensorsNum = 0;
    if (m_pGpu == 0)
    {
        m_pGpu = new LGpu();
    }

    return m_pGpu->GetTemperature(gpuTemp.SensorsNum, gpuTemp.Temp);
}

void CTemperatureProbe::GetDiskTemp(OUT DiskTempInforArray& diskTemp)
{
    diskTemp.Count = 0;

    for (int i = 0; i < 25; i++)
    {
        wchar_t diskDriveID[256] = {0};
        wsprintfW(diskDriveID, L"\\\\.\\PhysicalDrive%d", i);

        // �򿪴��̿�����
        LIDEDiskController diskController(diskDriveID);
        if (!diskController.DeviceExist())
            continue;

        // ��ȡSMART����
        unsigned char smartData[362] = {0};
        if (!diskController.GetSMARTData(smartData))
            continue;

        // ����SMART����
        LSMARTParser smartParser(smartData);
        unsigned int temp = 0;
        if (!smartParser.GetTemperature(temp))
            continue;

        diskTemp.Temp[diskTemp.Count] = temp;
        diskTemp.DiskDriveID[diskTemp.Count] = diskDriveID;

        diskTemp.Count += 1;
        if(diskTemp.Count >= MAX_DISKTEMP_NUMBER)
            break;

    }

}

TemperatureProbe::TemperatureProbe()
{
    m_pTemperatureProbe = new CTemperatureProbe();
}

TemperatureProbe::~TemperatureProbe()
{
    if (m_pTemperatureProbe != 0)
    {
        delete m_pTemperatureProbe;
        m_pTemperatureProbe = 0;
    }
}

bool TemperatureProbe::GetCpuTemp(OUT CpuTempInfor& cpuTemp)
{
    return m_pTemperatureProbe->GetCpuTemp(cpuTemp);
}

bool TemperatureProbe::GetGpuTemp(OUT GpuTempInfor& gpuTemp)
{
    return m_pTemperatureProbe->GetGpuTemp(gpuTemp);
}

void TemperatureProbe::GetDiskTemp(OUT DiskTempInforArray& diskTemp)
{
    m_pTemperatureProbe->GetDiskTemp(diskTemp);
}