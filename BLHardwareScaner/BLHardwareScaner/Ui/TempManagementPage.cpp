

#include "TempManagementPage.h"

#include "..\\Src\\TemperatureProbe.h"

#include <QtGui/QMessageBox>


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
    m_pTempRefreshTimer->start();
}

void TempManagementPage::hideEvent(QHideEvent* e)
{
    m_pTempRefreshTimer->stop();
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

    TemperatureProbe tempProbe;
    CpuTempInfor cpuTempInfor;
    bool bRet = tempProbe.GetCpuTemp(cpuTempInfor);
    if (bRet)
    {
        QString strCpuTempInfor = QString::fromStdWString(L"CPU: %1 C").arg(cpuTempInfor.CoreTemp[0]);
        m_tempLabelList[currentLabelIndex]->setText(strCpuTempInfor);
        currentLabelIndex++;
    }

    unsigned int gpuTemp = tempProbe.GetGpuTemp();
    if (0 != gpuTemp)
    {
        QString strGpuTempInfor = QString::fromStdWString(L"GPU: %1 C").arg(gpuTemp);
        m_tempLabelList[currentLabelIndex]->setText(strGpuTempInfor);
        currentLabelIndex++;
    }

    DiskTempInforArray diskTempInforArray;
    tempProbe.GetDiskTemp(diskTempInforArray);
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