
#include "DiskSpeedPage.h"

DiskSpeedPage::DiskSpeedPage(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this); 

    m_uiRatio = 1.0f;
    m_currentRuningIndex = 0;

    QObject::connect(ui.pushButtonTest, SIGNAL(clicked()), this, SLOT(TestButtonClicked()));

    m_testMonitorTimer.setInterval(500);
    QObject::connect(&m_testMonitorTimer, SIGNAL(timeout()), this, SLOT(TestMonitorTimerTimeout()));
}

DiskSpeedPage::~DiskSpeedPage()
{
    this->Clear();
}

void DiskSpeedPage::SetUIRatio(IN float ratio)
{
    m_uiRatio = ratio;
}

void DiskSpeedPage::showEvent(QShowEvent* e)
{
    this->Init();
}

void DiskSpeedPage::Init()
{
    this->Clear();

    ui.labelSeqWSpeed->setText("");
    ui.labelSeqRSpeed->setText("");
    ui.label4KRandWSpeed->setText("");
    ui.label4KRandRSpeed->setText("");

    // 测试项列表中增加一个开头项, 但不做实际测试
    IDiskSpeedTest* pTempTest = new LDiskSequenceTest();
    m_testItemList.push_back(pTempTest);

    IDiskSpeedTest* pSeqTest = new LDiskSequenceTest();
    m_testItemList.push_back(pSeqTest);

    IDiskSpeedTest* p4KRandTest = new LDisk4KRandomTest();
    m_testItemList.push_back(p4KRandTest);
}

void DiskSpeedPage::Clear()
{
    for (unsigned int i = 0; i < m_testItemList.size(); i++)
    {
        delete m_testItemList[i];
        m_testItemList[i] = 0;
    }

    m_testItemList.clear();
}

void DiskSpeedPage::TestButtonClicked()
{
    if (ui.pushButtonTest->text() == "StartTest")
    {
        m_testMonitorTimer.start();

        ui.labelSeqWSpeed->setText("");
        ui.labelSeqRSpeed->setText("");
        ui.label4KRandWSpeed->setText("");
        ui.label4KRandRSpeed->setText("");

        ui.pushButtonTest->setText("StopTest");

        m_currentRuningIndex = 0;
    }
    else if (ui.pushButtonTest->text() == "StopTest")
    {
        m_testMonitorTimer.stop();
        ui.pushButtonTest->setText("StartTest");

    }
}

void DiskSpeedPage::TestMonitorTimerTimeout()
{
    LDiskSpeedTestState state = m_testItemList[m_currentRuningIndex]->GetState();
    QString readSpeed = QString::fromStdString("%1").arg(state.ReadSpeed);
    QString writeSpeed = QString::fromStdString("%1").arg(state.WriteSpeed);

    if (m_currentRuningIndex == 1)
    {
        ui.labelSeqRSpeed->setText(readSpeed);
        ui.labelSeqWSpeed->setText(writeSpeed);
    }

    if (m_currentRuningIndex == 2)
    {
        ui.label4KRandRSpeed->setText(readSpeed);
        ui.label4KRandWSpeed->setText(writeSpeed);
    }

    if (state.TestDone)
    {
        m_currentRuningIndex++;
        if (m_currentRuningIndex >= m_testItemList.size())
        {
            m_testMonitorTimer.stop();
            ui.pushButtonTest->setText("StartTest");
        }
        else
        {
            m_testItemList[m_currentRuningIndex]->Start(L"D:\\test.temp");
        }
    }


}