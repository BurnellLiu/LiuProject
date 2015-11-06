

#include "TemperatureProbe.h"

#include "Cpu\\LCpuTemperature.h"
#include "Gpu\\Gpu.h"

#include "DiskController\\LDiskController.h"
#include "SMARTPaser\\LSMARTPaser.h"


TemperatureProbe::TemperatureProbe()
{
    m_pCpuTemperature = 0;
}

TemperatureProbe::~TemperatureProbe()
{
    if (m_pCpuTemperature != 0)
    {
        delete m_pCpuTemperature;
        m_pCpuTemperature = 0;
    }
}

bool TemperatureProbe::GetCpuTemp(OUT CpuTempInfor& cpuTemp)
{
    cpuTemp.CoreNum = 0;

    if (m_pCpuTemperature == 0)
    {
        m_pCpuTemperature = new LCpuTemperature();
    }

    return m_pCpuTemperature->Get(cpuTemp.CoreNum, cpuTemp.CoreTemp);
}

unsigned int TemperatureProbe::GetGpuTemp()
{
    static LNvGpu nvGpu;
    static LAMDGpu amdGpu;

    unsigned int temp = nvGpu.GetTemperature();
    if (temp != 0)
        return temp;

    temp = amdGpu.GetTemperature();
    if (temp != 0)
        return temp;

    return 0;
}

void TemperatureProbe::GetDiskTemp(OUT DiskTempInforArray& diskTemp)
{
    diskTemp.Count = 0;

    for (int i = 0; i < 25; i++)
    {
        wchar_t diskDriveID[256] = {0};
        wsprintfW(diskDriveID, L"\\\\.\\PhysicalDrive%d", i);

        // 打开磁盘控制器
        LIDEDiskController diskController(diskDriveID);
        if (!diskController.DeviceExist())
            continue;

        // 获取SMART数据
        unsigned char smartData[362] = {0};
        if (!diskController.GetSMARTData(smartData))
            continue;

        // 解析SMART数据
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