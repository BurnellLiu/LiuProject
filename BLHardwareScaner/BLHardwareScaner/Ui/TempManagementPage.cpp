

#include "TempManagementPage.h"

#include <QtGui/QMessageBox>

#include "..\\Src\\TemperatureProbe.h"
#include "..\\Src\\PerformanceCounter.h"
#include "..\\Src\\Log\\LLog.h"



/// @brief 温度仓库类
class TempHouse
{
public:
    TempHouse()
    {

    }
    ~TempHouse()
    {

    }

    const CpuTempInfor& GetCpuTemp() const
    {
        return s_cpuTempInfor;
    }

    unsigned int GetGpuTemp() const
    {
        return s_gpuTemp;
    }

    const DiskTempInforArray& GetDiskTemp() const
    {
        return s_diskTempInforArray;
    }

private:

    void SetCpuTemp(IN const CpuTempInfor& cpuTempInfor)
    {
        s_cpuTempInfor = cpuTempInfor;
    }

    void SetGpuTemp(IN unsigned int gpuTemp)
    {
        s_gpuTemp = gpuTemp;
    }

    void SetDiskTemp(IN const DiskTempInforArray& diskTempInforArray)
    {
        s_diskTempInforArray = diskTempInforArray;
    }

private:
    static CpuTempInfor s_cpuTempInfor; ///< CPU温度信息
    static unsigned int s_gpuTemp; ///< GPU温度信息
    static DiskTempInforArray s_diskTempInforArray; ///< 磁盘温度信息

    friend class ScanTempThread;
};

CpuTempInfor TempHouse::s_cpuTempInfor = {0};
unsigned int TempHouse::s_gpuTemp = 0;
DiskTempInforArray TempHouse::s_diskTempInforArray = {0};

/// @brief 性能仓库类
class PerformanceHouse
{
public:
    PerformanceHouse()
    {

    }

    ~PerformanceHouse()
    {

    }

    const MemoryPerformance& GetMemoryPerformance() const
    {
        return s_memoryPerformance;
    }

    const ProcessorPerformance& GetProcessorPerformance() const
    {
        return s_processorPerformance;
    }

private:

    void SetMemoryPerformance(IN MemoryPerformance& memoryPerf)
    {
        s_memoryPerformance = memoryPerf;
    }

    void SetProcessorPerformance(IN ProcessorPerformance& processorPerf)
    {
        s_processorPerformance = processorPerf;
    }

private:
    static MemoryPerformance s_memoryPerformance; ///< 内存性能
    static ProcessorPerformance s_processorPerformance; ///< 处理器性能

    friend class ScanPerformanceThread;
};

MemoryPerformance PerformanceHouse::s_memoryPerformance = {0};
ProcessorPerformance PerformanceHouse::s_processorPerformance = {0};

ScanTempThread::ScanTempThread()
{
    m_bStopThread = false;
}

ScanTempThread::~ScanTempThread()
{

}

void ScanTempThread::Stop()
{
    m_bStopThread = true;
}

void ScanTempThread::run()
{
    m_bStopThread = false;

    TemperatureProbe tempProbe;

    CpuTempInfor cpuTempInfor = {0};
    unsigned int gpuTemp = 0;
    DiskTempInforArray diskTempInforArray = {0};

    TempHouse tempHouse;

    int refreshCount = -1;
    while (!m_bStopThread)
    {
        refreshCount++;

        tempProbe.GetCpuTemp(cpuTempInfor);
        gpuTemp = tempProbe.GetGpuTemp();
        tempProbe.GetDiskTemp(diskTempInforArray);
       
        tempHouse.SetCpuTemp(cpuTempInfor);
        tempHouse.SetGpuTemp(gpuTemp);
        tempHouse.SetDiskTemp(diskTempInforArray);

        this->msleep(1000);

        // 每刷新10次写一次LOG
        if (refreshCount%10 != 0)
            continue;

        PrintLogW(L"Cpu Cores(Temperature): %u", cpuTempInfor.CoreNum);
        for (unsigned int i = 0; i < cpuTempInfor.CoreNum; i++)
        {
            PrintLogW(L"Cpu Cores: %u, Temp: %u", i, cpuTempInfor.CoreTemp[i]);
        }
        PrintLogW(L"Gpu Temp: %u", gpuTemp);
        PrintLogW(L"Disk Count(Temperature): %u", diskTempInforArray.Count);
        for (unsigned int i = 0; i < diskTempInforArray.Count; i++)
        {
            PrintLogW(L"Disk:%s, Temp: %u", diskTempInforArray.DiskDriveID[i].c_str(), diskTempInforArray.Temp[i]);
        }

        PrintLogW(L"");
        
    }


}

ScanPerformanceThread::ScanPerformanceThread()
{
    m_bStopThread = false;
}

ScanPerformanceThread::~ScanPerformanceThread()
{

}

void ScanPerformanceThread::Stop()
{
    m_bStopThread = true;
}

void ScanPerformanceThread::run()
{
    m_bStopThread = false;

    PerformanceCounter perfCounter;
    MemoryPerformance memoryPerf = {0};
    ProcessorPerformance processorPerf = {0};

    PerformanceHouse perfHouse;

    while (!m_bStopThread)
    {
        perfCounter.GetMemoryPerformance(memoryPerf);
        perfCounter.GetProcessorPerformance(processorPerf);

        perfHouse.SetMemoryPerformance(memoryPerf);
        perfHouse.SetProcessorPerformance(processorPerf);

        this->msleep(1000);
    }
}

TempManagementPage::TempManagementPage(IN QWidget *parent, IN Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    m_pTempRefreshTimer = new QTimer();
    m_pTempRefreshTimer->setInterval(1000);
    QObject::connect(m_pTempRefreshTimer, SIGNAL(timeout()), this, SLOT(UiRefreshTimerTimeout()));

}

TempManagementPage::~TempManagementPage()
{
    if (m_pTempRefreshTimer != NULL)
    {
        delete m_pTempRefreshTimer;
        m_pTempRefreshTimer = NULL;
    }
}

void TempManagementPage::showEvent(QShowEvent* e)
{
    m_scanTempThread.start();
    m_scanPerformanceThred.start();

    m_pTempRefreshTimer->start();
}

void TempManagementPage::hideEvent(QHideEvent* e)
{
    m_pTempRefreshTimer->stop();

    m_scanTempThread.Stop();
    m_scanPerformanceThred.Stop();
}

void TempManagementPage::UiRefreshTimerTimeout()
{
    this->RefreshUi();
}

void TempManagementPage::RefreshUi()
{
    TempHouse tempHouse;

    CpuTempInfor cpuTempInfor = tempHouse.GetCpuTemp();
    ui.progressBarCpuTemp->setValue(cpuTempInfor.CoreTemp[0]);

    unsigned int gpuTemp = tempHouse.GetGpuTemp();
    ui.progressBarGpuTemp->setValue(gpuTemp);

    DiskTempInforArray diskTempInforArray = tempHouse.GetDiskTemp();
    for (unsigned int i = 0; i < diskTempInforArray.Count; i++)
    {
        if (diskTempInforArray.DiskDriveID[i].find(L"0") != wstring::npos)
        {
            ui.progressBarDiskTemp->setValue(diskTempInforArray.Temp[i]);
            break;
        }
    }

    PerformanceHouse perfHouse;
    MemoryPerformance memoryPerf = perfHouse.GetMemoryPerformance();
    ProcessorPerformance processorPerf = perfHouse.GetProcessorPerformance();

    unsigned long memoryUsage = 0;
    if (memoryPerf.TotalSize != 0)
    {
        memoryUsage = 100-memoryPerf.AvailableSize * 100/memoryPerf.TotalSize;
    }

    ui.progressBarCpuUsage->setValue(processorPerf.LoadPercentage);
    ui.progressBarMemUsage->setValue(memoryUsage);
        
}

