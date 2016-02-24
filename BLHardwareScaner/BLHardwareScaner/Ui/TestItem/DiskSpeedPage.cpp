
#include "DiskSpeedPage.h"

#include "..\\Src\\Log\\LLog.h"

#define DISK_TEST_START QString::fromStdWString(L"Test Start")
#define DISK_TEST_STOP QString::fromStdWString(L"Test Stop")
#define DISK_SPEED_ZERO QString::fromStdWString(L"0.00")

DiskSpeedPage::DiskSpeedPage(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    m_pSeqTest = new LDiskSequenceTest();
    m_p4KRandTest = new LDisk4KRandomTest();

    ui.setupUi(this); 

    QObject::connect(ui.pushButtonTest, SIGNAL(clicked()), this, SLOT(TestButtonClicked()));

    m_seqTestTimer.setInterval(500);
    m_rand4KTestTimer.setInterval(500);

    QObject::connect(&m_seqTestTimer, SIGNAL(timeout()), this, SLOT(SeqTestMonitorTimer()));
    QObject::connect(&m_rand4KTestTimer, SIGNAL(timeout()), this, SLOT(Rand4KTestMonitorTimer()));
}

DiskSpeedPage::~DiskSpeedPage()
{
    if (m_pSeqTest != 0)
    {
        delete m_pSeqTest;
        m_pSeqTest = 0;
    }

    if (m_p4KRandTest != 0)
    {
        delete m_p4KRandTest;
        m_p4KRandTest = 0;
    }
}

void DiskSpeedPage::showEvent(QShowEvent* e)
{
    static bool s_intiDone = false;

    if (!s_intiDone)
    {
        ui.labelSeqWSpeed->setText(DISK_SPEED_ZERO);
        ui.labelSeqRSpeed->setText(DISK_SPEED_ZERO);
        ui.label4KRandWSpeed->setText(DISK_SPEED_ZERO);
        ui.label4KRandRSpeed->setText(DISK_SPEED_ZERO);

        ui.labelTestState->setText("");

        ui.pushButtonTest->setText(DISK_TEST_START);

        s_intiDone = true;
    }
    
}

void DiskSpeedPage::TestButtonClicked()
{
    if (ui.pushButtonTest->text() == DISK_TEST_START)
    {
        PrintLogW(L"");
        PrintLogW(L"Start Disk Speed Test");

        ui.labelSeqWSpeed->setText(DISK_SPEED_ZERO);
        ui.labelSeqRSpeed->setText(DISK_SPEED_ZERO);
        ui.label4KRandWSpeed->setText(DISK_SPEED_ZERO);
        ui.label4KRandRSpeed->setText(DISK_SPEED_ZERO);

        ui.pushButtonTest->setText(DISK_TEST_STOP);

        m_pSeqTest->Start(L"D:\\test.temp");
        m_seqTestTimer.start();

        ui.labelTestState->setText("Sequence Test Is Running...");
        PrintLogW(L"Start Disk Sequence Test");

    }
    else if (ui.pushButtonTest->text() == DISK_TEST_STOP)
    {
        PrintLogW(L"User Stop Disk Speed Test");

        m_pSeqTest->Stop();
        m_p4KRandTest->Stop();

        m_seqTestTimer.stop();
        m_rand4KTestTimer.stop();

        ui.pushButtonTest->setText(DISK_TEST_START);
        ui.labelTestState->setText("");
    }
}

void DiskSpeedPage::SeqTestMonitorTimer()
{
    LDiskSpeedTestState state = m_pSeqTest->GetState();
    QString readSpeed = QString::number(state.ReadSpeed, 'f', 2);
    QString writeSpeed = QString::number(state.WriteSpeed, 'f', 2);
    ui.labelSeqRSpeed->setText(readSpeed);
    ui.labelSeqWSpeed->setText(writeSpeed);

    // ²âÊÔÎ´½áÊø
    if (!state.TestDone)
        return;

    // ²âÊÔ½áÊø
    m_seqTestTimer.stop();

    // ²âÊÔÃ»·¢Éú´íÎóÔò½øÐÐ4KËæ»ú²âÊÔ
    if (state.Error == DST_NO_ERROR)
    {
        PrintLogW(L"Disk Sequence Test Is Done, Read Speed: %s MB/s, Write Speed: %s MB/s", 
            readSpeed.toStdWString().c_str(), 
            writeSpeed.toStdWString().c_str());

        m_p4KRandTest->Start(L"D:\\test.temp");
        m_rand4KTestTimer.start();
        
        ui.labelTestState->setText("4K Random Test Is Running...");

        PrintLogW(L"Start Disk 4K Random Test");
    }

    // ²âÊÔ·¢Éú´íÎó, ÏÔÊ¾´íÎóÏûÏ¢, Í£Ö¹²âÊÔ
    if (state.Error != DST_NO_ERROR)
    {
        PrintLogW(L"Disk Sequence Test Error, Message: %s", state.ErrorMsg.c_str());
        PrintLogW(L"Disk Sequence Test Error, Detailed Message: %s", state.ErrorMsgWindows.c_str());

        ui.labelTestState->setText(QString::fromStdWString(state.ErrorMsg));
        ui.pushButtonTest->setText(DISK_TEST_START);
    }
    
}

void DiskSpeedPage::Rand4KTestMonitorTimer()
{
    LDiskSpeedTestState state = m_p4KRandTest->GetState();
    QString readSpeed = QString::number(state.ReadSpeed, 'f', 2);
    QString writeSpeed = QString::number(state.WriteSpeed, 'f', 2);
    ui.label4KRandRSpeed->setText(readSpeed);
    ui.label4KRandWSpeed->setText(writeSpeed);

    if (!state.TestDone)
        return;

    m_rand4KTestTimer.stop();
    ui.pushButtonTest->setText(DISK_TEST_START);

    if (state.Error == DST_NO_ERROR)
    {
        PrintLogW(L"Disk 4K Random Test Is Done, Read Speed: %s MB/s, Write Speed: %s MB/s", 
            readSpeed.toStdWString().c_str(), 
            writeSpeed.toStdWString().c_str());

        ui.labelTestState->setText("");
    }
    else
    {
        PrintLogW(L"Disk 4K Random Test Error, Message: %s", state.ErrorMsg.c_str());
        PrintLogW(L"Disk 4K Random Test Error, Detailed Message: %s", state.ErrorMsgWindows.c_str());

        ui.labelTestState->setText(QString::fromStdWString(state.ErrorMsg));
    }
}