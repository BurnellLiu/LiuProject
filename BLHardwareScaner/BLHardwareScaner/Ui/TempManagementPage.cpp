

#include "TempManagementPage.h"

#include "..\\Src\\TemperatureProbe.h"

#include <QtGui/QMessageBox>

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

    while (!m_bStopThread)
    {
        tempProbe.GetCpuTemp(cpuTempInfor);
        gpuTemp = tempProbe.GetGpuTemp();
        tempProbe.GetDiskTemp(diskTempInforArray);

        tempHouse.SetCpuTemp(cpuTempInfor);
        tempHouse.SetGpuTemp(gpuTemp);
        tempHouse.SetDiskTemp(diskTempInforArray);

        this->msleep(1000);
    }


}

TempManagementPage::TempManagementPage(IN QWidget *parent, IN Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this);

    m_tempLabelList.append(ui.label_temp1);
    m_tempLabelList.append(ui.label_temp2);
    m_tempLabelList.append(ui.label_temp3);
    m_tempLabelList.append(ui.label_temp4);
    m_tempLabelList.append(ui.label_temp5);
    m_tempLabelList.append(ui.label_temp6);
    m_tempLabelList.append(ui.label_temp7);
    m_tempLabelList.append(ui.label_temp8);
    for (int i = 0; i < m_tempLabelList.size(); i++)
    {
        m_tempLabelList[i]->setText("");
    }

    m_pTempRefreshTimer = new QTimer();
    m_pTempRefreshTimer->setInterval(1000);
    QObject::connect(m_pTempRefreshTimer, SIGNAL(timeout()), this, SLOT(TempRefreshTimerTimeout()));

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

    m_pTempRefreshTimer->start();
}

void TempManagementPage::hideEvent(QHideEvent* e)
{
    m_pTempRefreshTimer->stop();
    m_scanTempThread.Stop();
}

void TempManagementPage::TempRefreshTimerTimeout()
{
    this->RefreshTemperature();
}

void TempManagementPage::RefreshTemperature()
{
    for (int i = 0; i < m_tempLabelList.size(); i++)
    {
        m_tempLabelList[i]->setText("");
    }

    int currentLabelIndex = 0;

    TempHouse tempHouse;

    CpuTempInfor cpuTempInfor = tempHouse.GetCpuTemp();
    if (cpuTempInfor.CoreNum > 0)
    {
        QString strCpuTempInfor = QString::fromStdWString(L"CPU: %1 C").arg(cpuTempInfor.CoreTemp[0]);
        m_tempLabelList[currentLabelIndex]->setText(strCpuTempInfor);
        currentLabelIndex++;
    }

    unsigned int gpuTemp = tempHouse.GetGpuTemp();
    if (0 != gpuTemp)
    {
        QString strGpuTempInfor = QString::fromStdWString(L"GPU: %1 C").arg(gpuTemp);
        m_tempLabelList[currentLabelIndex]->setText(strGpuTempInfor);
        currentLabelIndex++;
    }

    DiskTempInforArray diskTempInforArray = tempHouse.GetDiskTemp();
    if (1 == diskTempInforArray.Count)
    {
        QString strDiskTempInfor = QString::fromStdWString(L"Disk: %1 C").arg(diskTempInforArray.Temp[0]);
        m_tempLabelList[currentLabelIndex]->setText(strDiskTempInfor);

        currentLabelIndex++;
    }

    if (diskTempInforArray.Count > 1)
    {
        for (unsigned int i = 0; i < diskTempInforArray.Count; i++)
        {
            QString strDiskTempInfor = QString::fromStdWString(L"Disk%1: %2 C").arg(i + 1).arg(diskTempInforArray.Temp[0]);
            m_tempLabelList[currentLabelIndex]->setText(strDiskTempInfor);

            currentLabelIndex++;
        }
    }
        
}

