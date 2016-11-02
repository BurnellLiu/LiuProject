

#include "TemperatureProbe.h"

#include <vector>
using std::vector;

#include <Windows.h>

#include "DiskController\\LDiskController.h"
#include "SMARTPaser\\LSMARTPaser.h"

typedef bool (*GetCpuTempFun)(OUT unsigned int* pCoreNum, OUT unsigned int temp[MAX_PROCESSOR_PHYSICAL_CORE_NUM]);

typedef bool (*GetGpuTempFun)(OUT unsigned int* pSensorNum, OUT unsigned int temp[MAX_GPU_SENSORS_NUMBER]);

/// @brief �¶�̽��ʵ����
class CTemperatureProbe
{
public:
    /// @brief ���캯��
    CTemperatureProbe()
    {
        m_pGetCpuTemp = NULL;
        m_pGetGpuTemp = NULL;
        m_hCoreTemp = NULL;

        m_hCoreTemp = LoadLibraryW(L".\\CoreTemp\\CoreTemp.dll");

        if (m_hCoreTemp == NULL)
            return;

        m_pGetCpuTemp = (GetCpuTempFun)GetProcAddress(m_hCoreTemp, "GetCpuTemp");

        m_pGetGpuTemp = (GetGpuTempFun)GetProcAddress(m_hCoreTemp, "GetGpuTemp");

        this->ScanSataDiskId(m_sataDiskIdList);

    }

    /// @brief ��������
    ~CTemperatureProbe()
    {
        if (m_hCoreTemp != NULL)
        {
            FreeLibrary(m_hCoreTemp);
            m_hCoreTemp = NULL;
        }
    }

    /// @brief ��ȡCPU�¶�
    /// @param[out] OUT cpuTemp �洢CPU�¶���Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetCpuTemp(OUT CpuTempInfor& cpuTemp)
    {
        cpuTemp.CoreNum = 0;

        if (m_pGetCpuTemp == NULL)
            return false;

        return m_pGetCpuTemp(&cpuTemp.CoreNum, cpuTemp.CoreTemp);
    }

    /// @brief ��ȡGPU�¶�
    /// @param[out] OUT gpuTemp �洢GPU�¶���Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetGpuTemp(OUT GpuTempInfor& gpuTemp)
    {
        gpuTemp.SensorsNum = 0;

        if (m_pGetGpuTemp == NULL)
            return false;

        return m_pGetGpuTemp(&gpuTemp.SensorsNum, gpuTemp.Temp);
    }

    /// @brief ��ȡ�����¶�
    /// �÷���ֻ�ܻ�ȡSATA(IDE)���̵��¶�
    /// @param[out] diskTemp �洢�����¶�
    void GetDiskTemp(OUT DiskTempInforArray& diskTemp)
    {
        diskTemp.Count = 0;

        for (unsigned int i = 0; i < m_sataDiskIdList.size(); i++)
        {
            const wstring& diskDriveID = m_sataDiskIdList[i];

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

private:
    /// @brief ɨ��SATA����ID
    /// ֻ��SATA���ܻ�ȡSMART����, ���ܻ�ȡ���¶�
    /// @param[out] idList �洢����ID
    void ScanSataDiskId(OUT vector<wstring>& idList)
    {
        idList.clear();

        for (int i = 0; i < 25; i++)
        {
            wchar_t diskDriveId[256] = {0};
            wsprintfW(diskDriveId, L"\\\\.\\PhysicalDrive%d", i);

            // �򿪴��̿�����
            LIDEDiskController diskController(diskDriveId);
            if (!diskController.DeviceExist())
                continue;

            // ��ȡSMART����
            unsigned char smartData[362] = {0};
            if (!diskController.GetSMARTData(smartData))
                continue;

            idList.push_back(diskDriveId);
        }
    }

private:
    GetCpuTempFun m_pGetCpuTemp; ///< ��ȡCPU�¶Ⱥ���ָ��
    GetGpuTempFun m_pGetGpuTemp; ///< ��ȡGPU�¶Ⱥ���ָ��
    HMODULE m_hCoreTemp; ///< CoreTemp DLL���
    vector<wstring> m_sataDiskIdList; ///< �洢SATA����ID
};

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