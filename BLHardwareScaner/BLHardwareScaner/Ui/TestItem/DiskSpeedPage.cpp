
#include "DiskSpeedPage.h"

#include <QtCore/QFile>

#include "..\\Src\\Log\\LLog.h"
#include "..\\Src\\LHardwareInfor.h"
#include "..\\Src\\DiskController\\LDiskController.h"

#define DISK_TEST_START QString::fromStdWString(L"Test Start")
#define DISK_TEST_STOP QString::fromStdWString(L"Test Stop")
#define DISK_SPEED_ZERO QString::fromStdWString(L"0.00")

#define SEQ_TEST_FILENAME QString::fromStdWString(L"SquenceTest.temp");
#define RANDOM_4K_TEST_FILENAME QString::fromStdWString(L"Random4KTest.temp");

DiskSpeedPage::DiskSpeedPage(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);
    this->LoadQSS();

    m_pSeqTest = new LDiskSequenceTest();
    m_p4KRandTest = new LDisk4KRandomTest();

    QObject::connect(ui.pushButtonTest, SIGNAL(clicked()), this, SLOT(TestButtonClicked()));

    m_seqTestTimer.setInterval(500);
    m_rand4KTestTimer.setInterval(500);

    QObject::connect(&m_seqTestTimer, SIGNAL(timeout()), this, SLOT(SeqTestMonitorTimer()));
    QObject::connect(&m_rand4KTestTimer, SIGNAL(timeout()), this, SLOT(Rand4KTestMonitorTimer()));

    // 如果在showEvent中更新磁盘信息, 那么第一次显示测速UI的时候有卡顿现象
    this->UpdateDiskInformation();
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
        int diskIndex = ui.comboBoxDiskList->currentIndex();

        unsigned long long driveFreeSpace = 0;
        this->SelectLogicalDrive(m_diskLogicalNameList[diskIndex], &m_currentTestLogicalDrive, &driveFreeSpace);
        PrintLogW(L"");
        PrintLogW(L"Current Test Logical Drive: %s, Free Space: %I64u", 
            m_currentTestLogicalDrive.toStdWString().c_str(),
            driveFreeSpace);
        if (driveFreeSpace < (unsigned long long)(4) * 1024 * 1024 * 1024)
        {
            ui.labelTestState->setText(QString::fromStdWString(L"Disk Free Size Is Tool Small"));
            PrintLogW(L"Disk Free Size Is Tool Small, 4G Is Needed");
            return;
        }

        ui.labelSeqWSpeed->setText(DISK_SPEED_ZERO);
        ui.labelSeqRSpeed->setText(DISK_SPEED_ZERO);
        ui.label4KRandWSpeed->setText(DISK_SPEED_ZERO);
        ui.label4KRandRSpeed->setText(DISK_SPEED_ZERO);
        ui.pushButtonTest->setText(DISK_TEST_STOP);
        ui.labelTestState->setText("Sequence Test Is Running...");
        ui.comboBoxDiskList->setEnabled(false);
  
        PrintLogW(L"Start Disk Speed Test");
        
        QString testFilePath = m_currentTestLogicalDrive;
        testFilePath += QString::fromStdWString(L"\\");
        testFilePath += SEQ_TEST_FILENAME;
        PrintLogW(L"Sequence Test File Path: %s", testFilePath.toStdWString().c_str());

        m_pSeqTest->Start(testFilePath.toStdWString());
        m_seqTestTimer.start();

        PrintLogW(L"Start Disk Sequence Test");

    }
    else if (ui.pushButtonTest->text() == DISK_TEST_STOP)
    {
        ui.pushButtonTest->setText(DISK_TEST_START);
        ui.labelTestState->setText("");
        ui.comboBoxDiskList->setEnabled(true);

        PrintLogW(L"User Stop Disk Speed Test");

        m_pSeqTest->Stop();
        m_p4KRandTest->Stop();

        m_seqTestTimer.stop();
        m_rand4KTestTimer.stop();

        
    }
}

void DiskSpeedPage::SeqTestMonitorTimer()
{
    LDiskSpeedTestState state = m_pSeqTest->GetState();
    QString readSpeed = QString::number(state.ReadSpeed, 'f', 2);
    QString writeSpeed = QString::number(state.WriteSpeed, 'f', 2);
    ui.labelSeqRSpeed->setText(readSpeed);
    ui.labelSeqWSpeed->setText(writeSpeed);

    // 测试未结束
    if (!state.TestDone)
        return;

    // 测试结束
    m_seqTestTimer.stop();

    // 测试没发生错误则进行4K随机测试
    if (state.Error == DST_NO_ERROR)
    {
        ui.labelTestState->setText("4K Random Test Is Running...");

        PrintLogW(L"Disk Sequence Test Is Done, Read Speed: %s MB/s, Write Speed: %s MB/s", 
            readSpeed.toStdWString().c_str(), 
            writeSpeed.toStdWString().c_str());

        QString testFilePath = m_currentTestLogicalDrive;
        testFilePath += QString::fromStdWString(L"\\");
        testFilePath += RANDOM_4K_TEST_FILENAME;
        PrintLogW(L"Disk 4K Random Test File Path: %s", testFilePath.toStdWString().c_str());

        m_p4KRandTest->Start(testFilePath.toStdWString());
        m_rand4KTestTimer.start();
        
        PrintLogW(L"Start Disk 4K Random Test");
    }

    // 测试发生错误, 显示错误消息, 停止测试
    if (state.Error != DST_NO_ERROR)
    {
        ui.labelTestState->setText(QString::fromStdWString(state.ErrorMsg));
        ui.pushButtonTest->setText(DISK_TEST_START);
        ui.comboBoxDiskList->setEnabled(true);

        PrintLogW(L"Disk Sequence Test Error, Message: %s", state.ErrorMsg.c_str());
        PrintLogW(L"Disk Sequence Test Error, Detailed Message: %s", state.ErrorMsgWindows.c_str());
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
    ui.comboBoxDiskList->setEnabled(true);

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

void DiskSpeedPage::UpdateDiskInformation()
{
    m_diskLogicalNameList.clear();
    QVector<QString> diskModelNameList;

    const DiskInforArray& diskInforArray = LHardwareInfor::GetDiskInfor();
    for (unsigned int i = 0; i < diskInforArray.Count; i++)
    {
        DISK_TYPE diskType = diskInforArray.DiskType[i];
        if (diskType != FIXED_DISK)
            continue;

        m_diskLogicalNameList.push_back(QString::fromStdWString(diskInforArray.LogicalName[i]));
        QString modelName;
        switch (diskInforArray.FixedDiskType[i])
        {
        case FIXED_DISK_HDD:
            modelName += "(HDD)";
            break;
        case FIXED_DISK_SSD:
            modelName += "(SSD)";
            break;
        case FIXED_DISK_EMMC:
            modelName += "(EMMC)";
            break;
        case FIXED_DISK_RAID:
            modelName += "(RAID)";
            break;
        default:
            break;
        }
   

        modelName += QString::fromStdWString(diskInforArray.Model[i]);
        diskModelNameList.push_back(modelName);

    }

    ui.comboBoxDiskList->clear();
    for (int i = 0; i < diskModelNameList.size(); i++)
    {
        QString showName;
        showName += QString::fromStdWString(L"(");
        showName += m_diskLogicalNameList[i];
        showName += QString::fromStdWString(L")");
        showName.replace(QString::fromStdWString(L";"), QString::fromStdWString(L" "));
        showName += diskModelNameList[i];

        ui.comboBoxDiskList->addItem(showName);

    }

    if (diskModelNameList.size() != 0)
    {
        ui.comboBoxDiskList->setCurrentIndex(0);
    }
}

bool DiskSpeedPage::SelectLogicalDrive(
    IN const QString& diskLogicalName, 
    OUT QString* pDrive, 
    OUT unsigned long long* pDriveFreeSpace)
{
    if (diskLogicalName.isEmpty())
        return false;

    QStringList driveList = diskLogicalName.split(QString::fromStdWString(L";"));
    if (driveList.size() < 1)
        return false;

    QString driveName;
    unsigned long long freeSpaceMax = 0;
    for (int i = 0; i < driveList.size(); i++)
    {
        unsigned long long freeSpace = 0;
        GetLogicalDriveFreeSpace(driveList[i].toStdWString(), &freeSpace);
        if (freeSpace > freeSpaceMax)
        {
            freeSpaceMax = freeSpace;
            driveName = driveList[i];
        }
    }

    if (pDrive != 0)
    {
        (*pDrive) = driveName;
    }

    if (pDriveFreeSpace != 0)
    {
        (*pDriveFreeSpace) = freeSpaceMax;
    }

    return true;
}

void DiskSpeedPage::LoadQSS()
{
    QFile qssFile(".\\Qss\\Default\\DiskSpeedPage.qss");  
    qssFile.open(QFile::ReadOnly);  

    if(qssFile.isOpen())  
    {  
        QString qss = qssFile.readAll();
        qssFile.close();  

        this->setStyleSheet(qss);
    } 
}